asset = "ttrTrainRed"
sx = 20.4
dx = 0.8
nx = 15
sz = -19.75
dz = 2.5
nz = 3

vy = 0.5
for x in range(nx):
    vx = x*dx + sx
    for z in range(nz):
        vz = z*dz + sz
        print "\t\t{"
        print "\t\t\t\"Asset\":\""+ str(asset)+"\","
        print "\t\t\t\"Position\":["+str(vx)+","+str(vy)+","+str(vz)+"],"
        print "\t\t\t\"Rotation\":[0,0,0,1]"
        print "\t\t},"
        