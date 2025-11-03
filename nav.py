"""Made a high level navigation loop/steps:

    Sense environment -> build/refresh occupancy grid.
    Localize -> get position (x, y, θ).
    Plan -> find path to next waypoint.
    Control -> compute motor commands (v, ω).
    Repeat at TBD frequency.

Going forward:

    May use IR-SIM just for python simulation.
    Further refine plan.
    get more familiar with Astra.
        continue coding w/ Astra in mind.
"""
import math
import time
class Event():
    def __init__(self, ID):
        self.EventID=ID
def getGPSfix():
    x=0
    y=0
    theta=0
    return (x,y,theta)
class navigate():
    def __init__(self, x=0, y=0, theta=0):
        #get GPS fix
        self.x=x
        self.y=y
        self.theta=theta
        self.v=10
    def Localize(self):
        (x,y,theta)=getGPSfix()
        self.x=x
        self.y=y
        self.theta=theta
        return (x,y,theta)
    def Plan(self, x, y): # returns most direct path
        #find path to next waypoint
        # get position
        (x1,y1,theta1)=self.Localize()
        distance=math.sqrt(((x-x1)*(x-x1)+(y-y1)*(y-y1)))
        rads = math.atan(y-y1 / x-x1)
        turnRadians = rads-theta1 # find angle to turn
        #print(distance, turnRadians)
        return (distance, turnRadians)
    def Control(self, goalx, goaly):
        #Control -> compute motor commands (v, ω).
        #Repeat at TBD frequency.
        i=0
        obstacle = False
        
        while True:
            if i==0:
                (distance, turnRadians)=self.Plan(goalx,goaly)
                self.theta+=turnRadians
                # turn to object
                # if obstacle is in path turn and set new waypoint
                if obstacle == True: 
                    turnRadians += 30
                
                # if see obstacle turn until you don't see any
                # move forward
            
            self.x+=self.v*math.cos(self.theta)
            self.y+=self.v*math.sin(self.theta)
            time.sleep(0.1)
            if i%10 == 0: print(self.x, self.y, goalx, goaly)

            if distance < 2: break
            i+=1
            if i>200:i=0
        pass

# run navigate
nav=navigate()
nav.Control(200, 200)
