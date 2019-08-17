from random import random
from random import seed as set_seed
from math import atan2
import cairo

#  ~~~~~~~~  class for graph nodes ~~~~~~~~

class node:
    def __init__(self,x,y,z):
        self.x = x
        self.y = y
        self.z = z
        self.flux = 0
        self.slope = 0
        self.coast = False
        self.river = False
        self.mountain = False
        self.sea_neighbors = []


#  ~~~~~~~~  class for the terrain ~~~~~~~~

class terrain:
    def __init__(self,n,sealevel=0.5,seed=None):
        self.sealevel = sealevel

        # generate an evenly spaced triagonal grid of points,
        # jittered for randomness
        if (seed != None):
            set_seed(seed)
            
        m = int(2*n/(3**0.5)) + 2
        if m%2 == 0:  # force odd m
            m = m+1
        s = 1./(n-1)
        h = (3**0.5)*s/2

        nodes = []
        for y in range(m):
            row = []
            k = y%2
            for x in range(n+k):
                px = s*x - 0.5*s*k + s*(random()-0.5)
                py = h*y - h + h*(random()-0.5)
                row.append( node(px,py,0) )
            nodes.append(row)

        # build graph
        self.graph = {}

        # ...starting with the corners
        self.graph[nodes[0][0]]   = ( nodes[1][0],   nodes[0][1],   nodes[1][1]   )
        self.graph[nodes[-1][0]]  = ( nodes[-2][0],  nodes[-2][1], nodes[-1][1]  )
        self.graph[nodes[0][-1]]  = ( nodes[0][-2],  nodes[1][-2],  nodes[1][-1]  )
        self.graph[nodes[-1][-1]] = ( nodes[-1][-2], nodes[-2][-2], nodes[-2][-1] )
        
        #next, the edges
        # sides
        for y in range(1,m-1):
            if y%2 == 0:
                # even left
                self.graph[nodes[y][0]] = ( nodes[y+1][0], nodes[y+1][1], nodes[y][1], nodes[y-1][1], nodes[y-1][0] )
                # even right
                self.graph[nodes[y][-1]] = ( nodes[y+1][-2], nodes[y+1][-1], nodes[y-1][-1], nodes[y-1][-2], nodes[y][-2] )
            else:
                # odd left
                self.graph[nodes[y][0]] = ( nodes[y+1][0], nodes[y][1], nodes[y-1][0] )
                # odd right
                self.graph[nodes[y][-1]] = ( nodes[y+1][-1], nodes[y][-2], nodes[y-1][-1] )
            
        # top & bottom
        for x in range(1,n-1):
            # bottom
            self.graph[nodes[0][x]] = ( nodes[0][x-1], nodes[1][x], nodes[1][x+1], nodes[0][x+1] )
            # bottom
            self.graph[nodes[-1][x]] = ( nodes[-1][x-1], nodes[-2][x], nodes[-2][x+1], nodes[-1][x+1] )

        # the bulk of the graph
        for y in range(1,m-1):
            k = y%2
            for x in range(1,n+k-1):
                self.graph[nodes[y][x]] = ( nodes[y-1][x-k], nodes[y-1][x+1-k], nodes[y][x-1], nodes[y][x+1], nodes[y+1][x-k], nodes[y+1][x+1-k] )
                    

    def gen_terrain(self,N,maxradius):
        for i in range(N):
            cx = 3*random() - 1
            cy = 3*random() - 1
            cr = maxradius*random()
            for n in self.graph:
                if ( (n.x-cx)**2 + (n.y-cy)**2 ) <= cr:
                    n.z += 1 + 0.1*random()
        self.normalize()

    def normalize(self):
        zmax = 0
        zmin = 1e99
        for n in self.graph:
            if n.z > zmax:
                zmax = n.z
            if n.z < zmin:
                zmin = n.z
        for n in self.graph:
            n.z = (n.z-zmin)/(zmax-zmin)        

    def get_flux(self):
        """ compute water flux through each node """
        # iterate through points from top to bottom
        points = sorted(self.graph,key=lambda n: -n.z)
        
        for n in points:
            if n.z >= self.sealevel:
                n.flux += 1
                zmin = n.z
                zmin2 = 1e99
                min_n = None
                for n2 in self.graph[n]:
                    if n2.z < zmin:
                        zmin = n2.z
                        min_n = n2
                    if n2.z < zmin2:
                        zmin2 = n2.z
                if min_n != None:
                    min_n.flux += n.flux
                else:
                    n.flux = 0
                    n.geo = 'lake'
                    n.z = zmin2 + 1e-3
        self.normalize_flux()
    
    def erode(self,c):
        self.get_flux()
        for n in self.graph:
            n.z -= c*n.flux**0.5
            
    def clean_coasts(self):
        for n in self.graph:
            sea = 0
            land = 0
            for n2 in self.graph[n]:
                if n2.z > self.sealevel:
                    land += 1
                else:
                    sea += 1
            if sea > 4 or 4 < land:
                n.z = 2*self.sealevel - n.z

    def normalize_flux(self):
        """ normalize the water flux """
        fmax = 0
        fmin = 1e99
        for n in self.graph:
            if n.flux > fmax:
                fmax = n.flux
            if n.flux < fmin:
                fmin = n.flux
        for n in self.graph:
            n.flux = (n.flux-fmin)/(fmax-fmin)
            
    def minmax(self):
        zmin = 1e99
        zmax = 0
        for n in self.graph:
            if n.z > zmax:
                zmax = n.z
            if n.z < zmin:
                zmin = n.z
        return zmin, zmax                
            
    def render(self,fname):
        lw = 1
        scale = 1000
        w = scale
        h = scale
        surf = cairo.SVGSurface(fname,w,h)

        ct = cairo.Context(surf)
        ct.set_source_rgb(1,1,1)
        ct.rectangle(0,0,w,h)
        ct.fill()
        ct.set_line_width(lw)
        
        ct.set_source_rgb(1,0,0)
        
        for n in self.graph:
            if n.z > 0.8:
                n.mountain = True
            if n.z > self.sealevel:
                # find coastline nodes
                n.sea_neighbors = [ n2 for n2 in self.graph[n] if n2.z <= self.sealevel ]
                if len(n.sea_neighbors) < 5 and len(n.sea_neighbors) > 0:
                    n.coast = True
            if n.flux**0.5 > 0.3 and n.z >= self.sealevel:
                # find river nodes
                n.river = True

        for n in self.graph:
            if n.z < self.sealevel:
                ns = sorted(self.graph[n],key=lambda n1: atan2(n1.y-n.y,n1.x-n.x))
                ct.set_source_rgb(0,0,1)
                ct.move_to(scale*ns[0].x, h-scale*ns[0].y)
                for n1 in ns[1:]:
                    ct.line_to(scale*n1.x, h-scale*n1.y)
                ct.close_path()
                ct.fill()
                
            if n.mountain and random()>0.8:
                # render mountain nodes
                ct.set_source_rgb(0,0,0)
                ct.move_to(scale*n.x-5,h-scale*n.y+5)
                ct.line_to(scale*n.x,h-scale*n.y)
                ct.line_to(scale*n.x+5,h-scale*n.y+5)
                ct.stroke()
            if n.coast:
                # render coast nodes
                ct.set_source_rgb(0,0,0)
                for n2 in self.graph[n]:
                    # if the two points have a common sea neighbor
                    if n2.coast and not set(n.sea_neighbors).isdisjoint(n2.sea_neighbors):
                        ct.move_to(scale*n.x, h-scale*n.y)
                        ct.line_to(scale*n2.x, h-scale*n2.y)
                        ct.stroke()
            if n.river:
                # render river nodes
                ct.set_source_rgb(0,0,1)
                for n2 in t.graph[n]:
                    if n2.river:
                        ct.move_to(scale*n.x, h-scale*n.y)
                        ct.line_to(scale*n2.x, h-scale*n2.y)
                        ct.stroke()
        
        # finish & save
        surf.finish()


t = terrain(2**8, sealevel=0.3)
t.gen_terrain(1000,0.1)
for i in range(200):
    t.get_flux()
for i in range(100):
    t.erode(1e-3)
t.get_flux()
t.render('map.svg')
