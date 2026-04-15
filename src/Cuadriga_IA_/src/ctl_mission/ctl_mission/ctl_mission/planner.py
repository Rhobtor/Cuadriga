import numpy as np
from . import utils
#import scipy.spatial.transform.rotation as rot 
from . import optimization
from copy import copy


class pmm_traj_1d():
    def __init__(self, p0, v0, a, t1, T):

        self.p0 = p0
        self.v0 = v0
        self.a = a
        self.t1 = t1
        self.t2 = T - t1

        self.p1 = p0 + v0*t1 + 0.5*a*t1**2
        self.v1 = v0 + a*t1
        self.p2 = self.p1 + self.v1*self.t2 - 0.5*a*self.t2**2
        self.v2 = self.v1 - a*self.t2

    def eval(self, t):

        if t <= self.t1:
            p = self.p0 + self.v0*t + 0.5*self.a*t**2
            v = self.v0 + self.a*t
        elif t <= self.t1 + self.t2:
            p = self.p1 + self.v1*(t-self.t1) - 0.5*self.a*(t-self.t1)**2
            v = self.v1 - self.a*(t-self.t1)
        else:
            p = self.p2
            v = self.v2

        return p, v
         
class pmm_traj_3d():
    def __init__(self, p0, v0, a, t1, T):

        self.T = T

        self.traj_x = pmm_traj_1d(p0[0], v0[0], a[0], t1[0], T)
        self.traj_y = pmm_traj_1d(p0[1], v0[1], a[1], t1[1], T)
        self.traj_z = pmm_traj_1d(p0[2], v0[2], a[2], t1[2], T)
    
    def eval(self, t):
            
        p = np.zeros(3)
        v = np.zeros(3)

        p[0], v[0] = self.traj_x.eval(t)
        p[1], v[1] = self.traj_y.eval(t)
        p[2], v[2] = self.traj_z.eval(t)

        return p, v

class trajectory():
    def __init__(self, p0, v0, a, t1, T):

        self.num_pieces = len(T)
        self.pieces = []
        self.duration = np.sum(T)

        p = p0
        v = v0

        for i in range(self.num_pieces):
            self.pieces += [pmm_traj_3d(p, v, a[:,i], t1[:,i], T[i])]
            p, v = self.pieces[i].eval(T[i])

    def eval(self, t):
            
        assert t >= 0
        assert t <= self.duration

        current_t = 0
        for piece in self.pieces:
            if t <= piece.T + current_t:
                return piece.eval(t-current_t)
            current_t += piece.T


class Planner():
    def __init__(self, a_max = 26.0, N = 3):
        self.a_max = a_max*0.7
        self.N = N
        self.optimizer = optimization.Optimization(a = a_max, N = N)

    def reset(self, initialState, wp):
        self.initialPos = initialState[:3]
        self.initialVel = initialState[3:]
        self.wp = wp
        # self.wpori = wpori

    def initializations(self, vinit = 8.0):
        t1 = np.zeros([3,self.N])
        t2 = np.zeros([3,self.N])
        s = np.zeros([3,self.N])
        self.wp_vel0 = np.zeros([3,self.N])

        p0 = copy(self.initialPos)
        v0 = copy(self.initialVel)
        p2 = copy(self.wp[:,0])
        
        ############################## proponer mas adelante con una direccion wpori
        dirVec = p2 - p0
        v2 = vinit*dirVec/np.linalg.norm(dirVec)
        # self.wp_vel0[:,0] = copy(v2)
        ##############################

        for i in range(self.N-1):
            t1[:,i], t2[:,i], s[:,i], _ = utils.CalculateTwoPointsTraj(p0,v0,p2,v2,self.a_max)
            t1[:,i], t2[:,i], s[:,i], _ = utils.scalingTwoPointsTraj(p0,v0,p2,v2,t1[:,i],t2[:,i],s[:,i],self.a_max)
            p0 = copy(p2)
            v0 = copy(v2)
            p2 = self.wp[:,i+1]
            dirVec = p2 - p0
            v2 = vinit*dirVec/np.linalg.norm(dirVec)
            # self.wp_vel0[:,i+1] = copy(v2)
        
        i += 1
        t1[:,i], t2[:,i], s[:,i], _ = utils.CalculateTwoPointsTraj(p0,v0,p2,v2,self.a_max)
        t1[:,i], t2[:,i], s[:,i], _ = utils.scalingTwoPointsTraj(p0,v0,p2,v2,t1[:,i],t2[:,i],s[:,i],self.a_max)

        

        self.acc0 = s*self.a_max
        self.T0 = t1[0,:] + t2[0,:]
        self.t0 = t1

        self.optimizer.set_inputs(self.initialPos, self.initialVel, self.wp,  self.t0, self.T0, self.acc0)
        # return trajectory(self.initialPos, self.initialVel, self.acc0, self.t0, self.T0)
    
    def optimizeTraj(self):
        t, T, acc =self.optimizer.solve()        
        return trajectory(self.initialPos, self.initialVel, acc, t, T)

        

    


        


                    
