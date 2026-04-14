#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/Plugin.hh>
#include <ignition/math/Vector3.hh>

#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

namespace gazebo
{
class World2Map : public WorldPlugin
{
public:
  void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) override
  {
    this->world_ = _world;

    // === Parámetros SDF con valores por defecto ===
    this->min_x_ = _sdf->HasElement("min_x") ? _sdf->Get<double>("min_x") : -10.0;
    this->max_x_ = _sdf->HasElement("max_x") ? _sdf->Get<double>("max_x") :  10.0;
    this->min_y_ = _sdf->HasElement("min_y") ? _sdf->Get<double>("min_y") : -10.0;
    this->max_y_ = _sdf->HasElement("max_y") ? _sdf->Get<double>("max_y") :  10.0;

    this->resolution_ = _sdf->HasElement("resolution") ? _sdf->Get<double>("resolution") : 0.05; // m/px
    this->z_max_cast_ = _sdf->HasElement("z_max_cast") ? _sdf->Get<double>("z_max_cast") : 3.0;
    this->z_min_cast_ = _sdf->HasElement("z_min_cast") ? _sdf->Get<double>("z_min_cast") : -1.0;
    this->output_path_ = _sdf->HasElement("output_path") ? _sdf->Get<std::string>("output_path") : "/tmp/world_map";

    // Modelos a ignorar (separados por comas)
    if (_sdf->HasElement("ignore_contains"))
      this->ignore_contains_ = _sdf->Get<std::string>("ignore_contains");

    // Lanza una sola vez después de que el mundo esté estable
    this->updateConn_ = event::Events::ConnectWorldUpdateBegin(
      std::bind(&World2Map::OnUpdate, this));
  }

private:
  void OnUpdate()
  {
    // Ejecuta una vez
    if (this->done_)
      return;
    this->done_ = true;

    auto engine = this->world_->Physics();
    if (!engine)
    {
      gzerr << "[world2map] No physics engine.\n";
      return;
    }

    const size_t width  = static_cast<size_t>(std::ceil((max_x_ - min_x_) / resolution_));
    const size_t height = static_cast<size_t>(std::ceil((max_y_ - min_y_) / resolution_));

    gzmsg << "[world2map] Generando mapa " << width << "x" << height
          << " a " << resolution_ << " m/px\n";

    // Buffer PGM (0=ocupado negro, 254=libre blanco, 205=desconocido)
    std::vector<unsigned char> img(width * height, static_cast<unsigned char>(254));

    // Ray shape
    physics::RayShapePtr ray = boost::dynamic_pointer_cast<physics::RayShape>(
        engine->CreateShape("ray", physics::CollisionPtr()));

    const double ray_len = (z_max_cast_ - z_min_cast_);
    const double eps = 1e-6;

    for (size_t j = 0; j < height; ++j)
    {
      double y = min_y_ + (j + 0.5) * resolution_;
      for (size_t i = 0; i < width; ++i)
      {
        double x = min_x_ + (i + 0.5) * resolution_;

        ignition::math::Vector3d start(x, y, z_max_cast_);
        ignition::math::Vector3d end  (x, y, z_min_cast_);
        ray->SetPoints(start, end);

        double dist;
        std::string entity;
        ray->GetIntersection(dist, entity);

        bool hit = false;
        if (!entity.empty() && dist >= 0.0 && dist <= ray_len - eps)
        {
          // Ignora ground plane y patrones indicados
          if (entity.find("ground_plane") == std::string::npos)
          {
            if (!ignore_contains_.empty() && entity.find(ignore_contains_) != std::string::npos)
              hit = false;
            else
              hit = true;
          }
        }

        // Coordenadas de imagen: fila 0 = parte superior (y=max)
        size_t row_from_top = (height - 1) - j;
        size_t idx = row_from_top * width + i;
        img[idx] = hit ? static_cast<unsigned char>(0) : static_cast<unsigned char>(254);
      }
    }

    // Escribir PGM (binario P5)
    const std::string pgm = output_path_ + ".pgm";
    std::ofstream ofs(pgm, std::ios::binary);
    ofs << "P5\n" << width << " " << height << "\n255\n";
    ofs.write(reinterpret_cast<const char*>(img.data()), img.size());
    ofs.close();

    // Escribir YAML
    const std::string yaml = output_path_ + ".yaml";
    std::ofstream yfs(yaml);
    yfs << "image: " << pgm << "\n";
    yfs << "resolution: " << resolution_ << "\n";
    // Origen en el sistema de mapas de ROS: esquina inferior izquierda del PGM
    yfs << "origin: [" << min_x_ << ", " << min_y_ << ", 0.0]\n";
    yfs << "negate: 0\n";
    yfs << "occupied_thresh: 0.65\n";
    yfs << "free_thresh: 0.196\n";
    yfs.close();

    gzmsg << "[world2map] Mapas guardados en: " << pgm << " y " << yaml << "\n";
  }

private:
  physics::WorldPtr world_;
  event::ConnectionPtr updateConn_;
  bool done_{false};

  double min_x_{-10}, max_x_{10}, min_y_{-10}, max_y_{10};
  double resolution_{0.05};
  double z_max_cast_{3.0}, z_min_cast_{-1.0};
  std::string output_path_{"/tmp/world_map"};
  std::string ignore_contains_{};
};

GZ_REGISTER_WORLD_PLUGIN(World2Map)
} // namespace gazebo
