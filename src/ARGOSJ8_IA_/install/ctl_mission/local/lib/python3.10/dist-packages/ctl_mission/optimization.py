import casadi as ca
import numpy as np
from os import system

def point_mass_function_1d():

    p0 = ca.SX.sym('p0')
    v0 = ca.SX.sym('v0')
    a = ca.SX.sym('a')
    t1 = ca.SX.sym('t1')
    T = ca.SX.sym('T')

    p1 = p0 + v0*t1 + 0.5*a*t1**2
    v1 = v0 + a*t1
    t2 = T - t1
    p2 = p1 + v1*t2 - 0.5*a*t2**2
    v2 = v1 - a*t2

    p_f = ca.Function('p_f', [p0, v0, t1, T, a], [p2])
    v_f = ca.Function('v_f', [v0, t1, T, a], [v2])

    return p_f, v_f

class Optimization():

    def __init__(self, a = 26.0, N = 3):

        self.N = N
        p_f, v_f = point_mass_function_1d()
        
        pos = ca.SX.sym('pos', (3, N+1))
        vel = ca.SX.sym('vel', (3))
        acc = ca.SX.sym('acc', (3, N))
        t = ca.SX.sym('t', (3, N))
        T = ca.SX.sym('T', N)

        parameters = ca.vertcat(pos[:,0], vel)
        for i in range(N):
            parameters = ca.vertcat(parameters, pos[:,i+1])
        
        g = []
        variables = []
        self.lbg = []
        self.ubg = []
        self.lbx = []
        self.ubx = []
        f = 0
        v0 = vel

        for i in range(N):

            variables += [t[:,i]]
            self.lbx += [0 for _ in range(3)]
            self.ubx += [ca.inf for _ in range(3)]
            variables += [T[i]]
            self.lbx += [0]
            self.ubx += [ca.inf]
            variables += [acc[:,i]]
            self.lbx += [-ca.inf for _ in range(3)]
            self.ubx += [ca.inf for _ in range(3)]

            g += [ca.sumsqr(acc[:,i])]
            self.lbg += [0]
            self.ubg += [a**2]
            for axis in range(3):
                g += [(pos[axis,i+1] - p_f(pos[axis,i], v0[axis], t[axis,i], T[i], acc[axis,i]))]
                self.lbg += [0]
                self.ubg += [0]
                g += [T[i] - t[axis,i]]
                self.lbg += [0]
                self.ubg += [ca.inf]
                v0[axis] = v_f(v0[axis], t[axis,i], T[i], acc[axis,i])

        
            f += 500*T[i]


        nlp = {'x': ca.vertcat(*variables), 'f': f, 'g': ca.vertcat(*g), 'p': parameters}

        print(parameters)
        print(ca.vertcat(*variables))

        ipopt_options = {
             'verbose': False, \
             # "ipopt.tol": 1e-4,
             # "ipopt.acceptable_tol": 1e-4,
             "ipopt.max_iter": 100,
             # "ipopt.warm_start_init_point": "yes",
             "ipopt.print_level": 0, 
             "print_time": False
        }
        solver = ca.nlpsol('solver', 'ipopt', nlp, ipopt_options)

        cname = solver.generate_dependencies("solver.c")
        system('gcc -fPIC -shared -O3 ' + cname + ' -o ' + './solver.so')
        self.solver = ca.nlpsol("vel", "ipopt", './solver.so', ipopt_options)

        '''
        sqp_options = {
            "qpsol": "qpoases", \
            "hessian_approximation": "gauss-newton", \
            'print_header': False, \
            'print_iteration': False, \
            'print_status': False, \
            'qpsol_options.printLevel': 'none', \
            "print_time": False, \
            "max_iter": 100, 
            "tol_du": 1e-2,
            "tol_pr": 1e-2,
            "qpsol_options": {"sparse":True, "hessian_type": "posdef", "numRefinementSteps":1} 
        }
        solver = ca.nlpsol('vel', 'sqpmethod', nlp, sqp_options)
        
        cname = solver.generate_dependencies("solver.c")
        system('gcc -fPIC -shared -O3 ' + cname + ' -o ' + './solver.so')
        self.solver = ca.nlpsol("vel", "sqpmethod", './solver.so', sqp_options)
        '''        

    def set_inputs(self, initial_pos, initial_vel, wp, t0, T0, acc0):
            
        self.parameters_solver = ca.vertcat(initial_pos, initial_vel)
        for i in range(self.N):
            self.parameters_solver = ca.vertcat(self.parameters_solver, wp[:,i])

        initial_guess = []
        for i in range(self.N):
            initial_guess += [t0[:,i]]
            initial_guess += [T0[i]]
            initial_guess += [acc0[:,i]]

        self.initial_guess = ca.vertcat(*initial_guess).full().ravel()

    def solve(self):

        sol = self.solver(x0 = self.initial_guess, lbx = self.lbx, ubx = self.ubx, lbg = self.lbg, ubg = self.ubg, p = self.parameters_solver)

        acc = np.zeros((3, self.N))
        t = np.zeros((3, self.N))
        T = np.zeros(self.N)

        solution = sol['x'].full()

        for i in range(self.N):
            t[:,i] = solution[7*i:7*i+3].ravel()
            T[i] = solution[3+7*i:7*i+4]
            acc[:,i] = solution[4+7*i:7*i+7].ravel()

        
        return t, T, acc 
    
