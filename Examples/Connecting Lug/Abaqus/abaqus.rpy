# -*- coding: mbcs -*-
#
# Abaqus/CAE Release 2020 replay file
# Internal Version: 2019_09_13-18.49.31 163176
# Run by lenovo on Fri Oct  2 17:45:07 2026
#

# from driverUtils import executeOnCaeGraphicsStartup
# executeOnCaeGraphicsStartup()
#: Executing "onCaeGraphicsStartup()" in the site directory ...
from abaqus import *
from abaqusConstants import *
session.Viewport(name='Viewport: 1', origin=(0.0, 0.0), width=208.28125, 
    height=123.305557250977)
session.viewports['Viewport: 1'].makeCurrent()
session.viewports['Viewport: 1'].maximize()
from caeModules import *
from driverUtils import executeOnCaeStartup
executeOnCaeStartup()
session.viewports['Viewport: 1'].partDisplay.geometryOptions.setValues(
    referenceRepresentation=ON)
openMdb(pathName='D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.cae')
#: The model database "D:\DOCTORAT\GABES Simulations\Connecting Lug\Abaqus\Lug.cae" has been opened.
session.viewports['Viewport: 1'].setValues(displayedObject=None)
p = mdb.models['Model-1'].parts['Lug']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
session.viewports['Viewport: 1'].view.setValues(nearPlane=214.12, 
    farPlane=381.147, width=212.35, height=91.061, viewOffsetX=5.38808, 
    viewOffsetY=0.564816)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
o3 = session.openOdb(
    name='D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb')
#: Model: D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       1
#: Number of Node Sets:          1
#: Number of Steps:              1
session.viewports['Viewport: 1'].setValues(displayedObject=o3)
session.viewports['Viewport: 1'].makeCurrent()
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].setValues(
    displayedObject=session.odbs['D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb'])
session.viewports['Viewport: 1'].assemblyDisplay.setValues(
    optimizationTasks=OFF, geometricRestrictions=OFF, stopConditions=OFF)
session.viewports['Viewport: 1'].view.setValues(nearPlane=212.58, 
    farPlane=382.687, width=224.279, height=96.1765, viewOffsetX=-16.9356, 
    viewOffsetY=-6.28104)
session.viewports['Viewport: 1'].view.setValues(nearPlane=217.812, 
    farPlane=386.69, width=229.799, height=98.5437, cameraPosition=(189.448, 
    114.019, 221.5), cameraUpVector=(-0.479743, 0.774028, -0.413191), 
    cameraTarget=(4.30699, 14.8639, 10.605), viewOffsetX=-17.3524, 
    viewOffsetY=-6.43564)
session.viewports['Viewport: 1'].view.setValues(nearPlane=223.212, 
    farPlane=381.29, width=172.832, height=74.1146, viewOffsetX=-8.91998, 
    viewOffsetY=3.73773)
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].view.setValues(nearPlane=222.757, 
    farPlane=384.877, width=195.201, height=83.7071, viewOffsetX=-8.79972, 
    viewOffsetY=3.38123)
session.viewports['Viewport: 1'].view.setValues(nearPlane=256.658, 
    farPlane=363.513, width=224.908, height=96.4464, cameraPosition=(-22.9217, 
    142.63, 289.705), cameraUpVector=(-0.276468, 0.660258, -0.698301), 
    cameraTarget=(2.66401, 18.6936, 20.3152), viewOffsetX=-10.1389, 
    viewOffsetY=3.89582)
session.viewports['Viewport: 1'].view.setValues(nearPlane=244.28, 
    farPlane=374.732, width=214.061, height=91.795, cameraPosition=(-92.3446, 
    -51.6864, 299.695), cameraUpVector=(-0.141862, 0.970897, -0.19296), 
    cameraTarget=(-0.729967, 8.90803, 23.071), viewOffsetX=-9.64993, 
    viewOffsetY=3.70793)
session.viewports['Viewport: 1'].view.setValues(nearPlane=256.019, 
    farPlane=358.962, width=224.348, height=96.2064, cameraPosition=(40.8707, 
    53.4868, 311.384), cameraUpVector=(0.148052, 0.857194, -0.493254), 
    cameraTarget=(2.94953, 7.93956, 19.711), viewOffsetX=-10.1137, 
    viewOffsetY=3.88612)
session.viewports['Viewport: 1'].view.setValues(nearPlane=237.671, 
    farPlane=372.643, width=208.27, height=89.3118, cameraPosition=(131.528, 
    27.6413, 283.71), cameraUpVector=(-0.285055, 0.913199, -0.291224), 
    cameraTarget=(6.56326, 11.288, 14.0766), viewOffsetX=-9.3889, 
    viewOffsetY=3.60762)
session.viewports['Viewport: 1'].view.setValues(nearPlane=241.005, 
    farPlane=369.31, width=211.192, height=90.5647, viewOffsetX=-3.87173, 
    viewOffsetY=4.34526)
session.viewports['Viewport: 1'].view.setValues(nearPlane=239.453, 
    farPlane=370.861, width=209.833, height=89.9816, cameraPosition=(131.307, 
    27.358, 283.83), cameraUpVector=(-0.241553, 0.918953, -0.311735), 
    cameraTarget=(6.34236, 11.0047, 14.1962), viewOffsetX=-3.8468, 
    viewOffsetY=4.31728)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
session.viewports['Viewport: 1'].view.setValues(nearPlane=223.157, 
    farPlane=372.111, width=126.406, height=54.3795, viewOffsetX=-2.72573, 
    viewOffsetY=8.66831)
p = mdb.models['Model-1'].parts['Lug']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
session.viewports['Viewport: 1'].partDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].partDisplay.meshOptions.setValues(
    meshTechnique=ON)
session.viewports['Viewport: 1'].partDisplay.geometryOptions.setValues(
    referenceRepresentation=OFF)
session.viewports['Viewport: 1'].view.setValues(nearPlane=218.247, 
    farPlane=377.02, width=179.774, height=77.3378, viewOffsetX=13.893, 
    viewOffsetY=16.3487)
p = mdb.models['Model-1'].parts['Lug']
p.deleteMesh()
session.viewports['Viewport: 1'].view.setValues(nearPlane=218.492, 
    farPlane=376.776, width=159.026, height=68.4123, viewOffsetX=17.9172, 
    viewOffsetY=3.23105)
elemType1 = mesh.ElemType(elemCode=C3D20R, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=C3D15, elemLibrary=STANDARD)
elemType3 = mesh.ElemType(elemCode=C3D10, elemLibrary=STANDARD, 
    secondOrderAccuracy=OFF, distortionControl=DEFAULT)
p = mdb.models['Model-1'].parts['Lug']
c = p.cells
cells = c.getSequenceFromMask(mask=('[#3f ]', ), )
pickedRegions =(cells, )
p.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2, 
    elemType3))
p = mdb.models['Model-1'].parts['Lug']
p.seedPart(size=1.5, deviationFactor=0.1, minSizeFactor=0.1)
p = mdb.models['Model-1'].parts['Lug']
p.seedPart(size=2.5, deviationFactor=0.1, minSizeFactor=0.1)
p = mdb.models['Model-1'].parts['Lug']
p.generateMesh()
session.viewports['Viewport: 1'].view.setValues(nearPlane=218.247, 
    farPlane=377.02, width=179.774, height=77.3378, viewOffsetX=12.6619, 
    viewOffsetY=10.5657)
a = mdb.models['Model-1'].rootAssembly
a.regenerate()
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
mdb.jobs['Lug'].submit(consistencyChecking=OFF)
#: The job input file "Lug.inp" has been submitted for analysis.
#: Job Lug: Analysis Input File Processor completed successfully.
#: Job Lug: Abaqus/Standard completed successfully.
#: Job Lug completed successfully. 
session.viewports['Viewport: 1'].view.setValues(nearPlane=214.198, 
    farPlane=381.07, width=211.75, height=95.5122, viewOffsetX=15.1776, 
    viewOffsetY=16.6866)
o3 = session.openOdb(
    name='D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb')
#: Model: D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       1
#: Number of Node Sets:          1
#: Number of Steps:              1
session.viewports['Viewport: 1'].setValues(displayedObject=o3)
session.viewports['Viewport: 1'].makeCurrent()
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].view.setValues(nearPlane=218.133, 
    farPlane=382.604, width=179.679, height=77.051, viewOffsetX=11.1818, 
    viewOffsetY=-0.740152)
session.viewports['Viewport: 1'].odbDisplay.contourOptions.setValues(
    maxValue=603.952, minValue=3.20287, showMaxLocation=ON)
session.viewports['Viewport: 1'].view.setValues(nearPlane=214.276, 
    farPlane=386.461, width=187.768, height=80.5199, viewOffsetX=-0.589281, 
    viewOffsetY=-6.11662)
session.viewports['Viewport: 1'].view.setValues(nearPlane=219.685, 
    farPlane=391.351, width=192.508, height=82.5525, cameraPosition=(254.003, 
    36.1229, 174.112), cameraUpVector=(-0.433052, 0.897616, -0.0821696), 
    cameraTarget=(5.41728, 11.9024, 12.2346), viewOffsetX=-0.604156, 
    viewOffsetY=-6.27103)
session.viewports['Viewport: 1'].view.setValues(nearPlane=262.078, 
    farPlane=347.639, width=229.657, height=98.483, cameraPosition=(-2.59429, 
    -52.4657, 308.369), cameraUpVector=(-0.0446958, 0.990443, -0.130478), 
    cameraTarget=(0.146369, 9.47725, 17.2656), viewOffsetX=-0.720742, 
    viewOffsetY=-7.48117)
session.viewports['Viewport: 1'].view.setValues(nearPlane=220.578, 
    farPlane=385.466, width=193.291, height=82.8882, cameraPosition=(-197.882, 
    -21.271, 239.394), cameraUpVector=(0.10099, 0.970638, -0.218319), 
    cameraTarget=(-5.28658, 10.3615, 14.6892), viewOffsetX=-0.606613, 
    viewOffsetY=-6.29653)
session.viewports['Viewport: 1'].view.setValues(nearPlane=225.504, 
    farPlane=380.54, width=197.608, height=84.7393, cameraPosition=(-196.992, 
    -21.3799, 240.142), cameraUpVector=(0.197185, 0.970911, -0.135831), 
    cameraTarget=(-4.39609, 10.2526, 15.4371), viewOffsetX=-0.62016, 
    viewOffsetY=-6.43715)
session.viewports['Viewport: 1'].view.setValues(nearPlane=227.729, 
    farPlane=376.445, width=199.558, height=85.5755, cameraPosition=(164.892, 
    61.1572, 256.323), cameraUpVector=(-0.242802, 0.873741, -0.421455), 
    cameraTarget=(1.72264, 11.6881, 12.367), viewOffsetX=-0.62628, 
    viewOffsetY=-6.50067)
session.viewports['Viewport: 1'].view.setValues(nearPlane=227.697, 
    farPlane=376.476, width=187.558, height=80.4297, viewOffsetX=-2.71363, 
    viewOffsetY=-4.21806)
p = mdb.models['Model-1'].parts['Lug']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
session.viewports['Viewport: 1'].view.setValues(nearPlane=227.666, 
    farPlane=367.601, width=94.9475, height=40.846, viewOffsetX=-5.70022, 
    viewOffsetY=17.2871)
p = mdb.models['Model-1'].parts['Lug']
p.deleteMesh()
session.viewports['Viewport: 1'].view.setValues(nearPlane=215.514, 
    farPlane=379.753, width=200.908, height=86.4299, viewOffsetX=40.6766, 
    viewOffsetY=5.97066)
elemType1 = mesh.ElemType(elemCode=C3D8R, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=C3D6, elemLibrary=STANDARD)
elemType3 = mesh.ElemType(elemCode=C3D4, elemLibrary=STANDARD, 
    secondOrderAccuracy=OFF, distortionControl=DEFAULT)
p = mdb.models['Model-1'].parts['Lug']
c = p.cells
cells = c.getSequenceFromMask(mask=('[#3f ]', ), )
pickedRegions =(cells, )
p.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2, 
    elemType3))
elemType1 = mesh.ElemType(elemCode=C3D8R, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=C3D6, elemLibrary=STANDARD)
elemType3 = mesh.ElemType(elemCode=C3D4, elemLibrary=STANDARD, 
    secondOrderAccuracy=OFF, distortionControl=DEFAULT)
p = mdb.models['Model-1'].parts['Lug']
c = p.cells
cells = c.getSequenceFromMask(mask=('[#3f ]', ), )
pickedRegions =(cells, )
p.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2, 
    elemType3))
p = mdb.models['Model-1'].parts['Lug']
p.generateMesh()
session.viewports['Viewport: 1'].view.setValues(nearPlane=215.816, 
    farPlane=379.451, width=177.772, height=76.4765, viewOffsetX=28.3435, 
    viewOffsetY=-2.35186)
a = mdb.models['Model-1'].rootAssembly
a.regenerate()
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
mdb.jobs['Lug'].submit(consistencyChecking=OFF)
#: The job input file "Lug.inp" has been submitted for analysis.
#: Job Lug: Analysis Input File Processor completed successfully.
#: Job Lug: Abaqus/Standard completed successfully.
#: Job Lug completed successfully. 
o3 = session.openOdb(
    name='D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb')
#: Model: D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       1
#: Number of Node Sets:          1
#: Number of Steps:              1
session.viewports['Viewport: 1'].setValues(displayedObject=o3)
session.viewports['Viewport: 1'].makeCurrent()
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].view.setValues(nearPlane=210.799, 
    farPlane=389.955, width=236.596, height=101.458, viewOffsetX=-1.56019, 
    viewOffsetY=8.01497)
session.viewports['Viewport: 1'].view.setValues(nearPlane=231.056, 
    farPlane=368.248, width=259.332, height=111.208, cameraPosition=(55.6174, 
    176.715, 252.245), cameraUpVector=(-0.129034, 0.597336, -0.791543), 
    cameraTarget=(-1.55898, 11.3773, 11.455), viewOffsetX=-1.71012, 
    viewOffsetY=8.78518)
session.viewports['Viewport: 1'].view.setValues(nearPlane=233.791, 
    farPlane=356.586, width=262.402, height=112.525, cameraPosition=(51.4479, 
    65.411, 294.939), cameraUpVector=(-0.240111, 0.851031, -0.467004), 
    cameraTarget=(-0.208878, 9.51791, 7.2012), viewOffsetX=-1.73037, 
    viewOffsetY=8.88919)
session.viewports['Viewport: 1'].view.setValues(nearPlane=233.376, 
    farPlane=357.002, width=261.936, height=112.325, cameraPosition=(49.4376, 
    64.8667, 295.406), cameraUpVector=(-0.0677363, 0.863192, -0.500312), 
    cameraTarget=(-2.21917, 8.97358, 7.66784), viewOffsetX=-1.7273, 
    viewOffsetY=8.87339)
session.viewports['Viewport: 1'].view.setValues(nearPlane=243.26, 
    farPlane=347.118, width=200.378, height=85.9272, viewOffsetX=4.481, 
    viewOffsetY=7.20773)
session.viewports['Viewport: 1'].view.setValues(nearPlane=241.942, 
    farPlane=348.436, width=199.293, height=85.4617, viewOffsetX=-10.2998, 
    viewOffsetY=-2.1688)
session.viewports['Viewport: 1'].view.setValues(nearPlane=244.909, 
    farPlane=345.469, width=167.56, height=71.8539, viewOffsetX=-7.44732, 
    viewOffsetY=1.87567)
p = mdb.models['Model-1'].parts['Lug']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
session.viewports['Viewport: 1'].view.setValues(nearPlane=220.915, 
    farPlane=374.353, width=142.074, height=61.1196, viewOffsetX=15.7597, 
    viewOffsetY=4.32481)
p = mdb.models['Model-1'].parts['Lug']
p.deleteMesh()
session.viewports['Viewport: 1'].view.setValues(nearPlane=215.817, 
    farPlane=379.451, width=177.772, height=76.4768, viewOffsetX=32.6148, 
    viewOffsetY=0.210926)
elemType1 = mesh.ElemType(elemCode=C3D20R, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=C3D15, elemLibrary=STANDARD)
elemType3 = mesh.ElemType(elemCode=C3D10, elemLibrary=STANDARD)
p = mdb.models['Model-1'].parts['Lug']
c = p.cells
cells = c.getSequenceFromMask(mask=('[#3f ]', ), )
pickedRegions =(cells, )
p.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2, 
    elemType3))
p = mdb.models['Model-1'].parts['Lug']
p.seedPart(size=3.5, deviationFactor=0.1, minSizeFactor=0.1)
p = mdb.models['Model-1'].parts['Lug']
p.generateMesh()
session.viewports['Viewport: 1'].view.setValues(nearPlane=219.513, 
    farPlane=375.754, width=169.968, height=73.1196, viewOffsetX=26.4027, 
    viewOffsetY=4.70464)
a = mdb.models['Model-1'].rootAssembly
a.regenerate()
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
mdb.jobs['Lug'].submit(consistencyChecking=OFF)
#: The job input file "Lug.inp" has been submitted for analysis.
#: Job Lug: Analysis Input File Processor completed successfully.
#: Job Lug: Abaqus/Standard completed successfully.
#: Job Lug completed successfully. 
o3 = session.openOdb(
    name='D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb')
#: Model: D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       1
#: Number of Node Sets:          1
#: Number of Steps:              1
session.viewports['Viewport: 1'].setValues(displayedObject=o3)
session.viewports['Viewport: 1'].makeCurrent()
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
p = mdb.models['Model-1'].parts['Lug']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
p = mdb.models['Model-1'].parts['Lug']
p.deleteMesh()
p = mdb.models['Model-1'].parts['Lug']
c = p.cells
pickedRegions = c.getSequenceFromMask(mask=('[#3f ]', ), )
p.setMeshControls(regions=pickedRegions, elemShape=HEX, technique=STRUCTURED)
elemType1 = mesh.ElemType(elemCode=C3D8R)
elemType2 = mesh.ElemType(elemCode=C3D6)
elemType3 = mesh.ElemType(elemCode=C3D4)
p = mdb.models['Model-1'].parts['Lug']
c = p.cells
cells = c.getSequenceFromMask(mask=('[#3f ]', ), )
pickedRegions =(cells, )
p.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2, 
    elemType3))
elemType1 = mesh.ElemType(elemCode=C3D20R, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=C3D15, elemLibrary=STANDARD)
elemType3 = mesh.ElemType(elemCode=C3D10, elemLibrary=STANDARD)
p = mdb.models['Model-1'].parts['Lug']
c = p.cells
cells = c.getSequenceFromMask(mask=('[#3f ]', ), )
pickedRegions =(cells, )
p.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2, 
    elemType3))
p = mdb.models['Model-1'].parts['Lug']
p.seedPart(size=7.0, deviationFactor=0.1, minSizeFactor=0.1)
p = mdb.models['Model-1'].parts['Lug']
p.generateMesh()
session.viewports['Viewport: 1'].view.setValues(nearPlane=218.249, 
    farPlane=377.018, width=179.776, height=77.3387, viewOffsetX=28.7455, 
    viewOffsetY=6.62215)
a = mdb.models['Model-1'].rootAssembly
a.regenerate()
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
mdb.jobs['Lug'].submit(consistencyChecking=OFF)
#: The job input file "Lug.inp" has been submitted for analysis.
#: Job Lug: Analysis Input File Processor completed successfully.
#: Job Lug: Abaqus/Standard completed successfully.
#: Job Lug completed successfully. 
o3 = session.openOdb(
    name='D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb')
#: Model: D:/DOCTORAT/GABES Simulations/Connecting Lug/Abaqus/Lug.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       1
#: Number of Node Sets:          1
#: Number of Steps:              1
session.viewports['Viewport: 1'].setValues(displayedObject=o3)
session.viewports['Viewport: 1'].makeCurrent()
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].view.setValues(nearPlane=212.425, 
    farPlane=388.342, width=224.115, height=96.1062, viewOffsetX=11.9478, 
    viewOffsetY=0.254008)
session.viewports['Viewport: 1'].view.setValues(nearPlane=212.204, 
    farPlane=385.118, width=223.882, height=96.0064, cameraPosition=(174.607, 
    140.965, 211.942), cameraUpVector=(-0.57735, 0.685225, -0.443997), 
    cameraTarget=(2.76863, 8.8169, 7.99641), viewOffsetX=11.9354, 
    viewOffsetY=0.253744)
session.viewports['Viewport: 1'].view.setValues(nearPlane=232.316, 
    farPlane=340.979, width=245.101, height=105.105, cameraPosition=(5.56939, 
    140.965, 264.966), cameraUpVector=(-0.176956, 0.685225, -0.706508), 
    cameraTarget=(-2.524, 8.8169, -1.59871), viewOffsetX=13.0666, 
    viewOffsetY=0.277793)
session.viewports['Viewport: 1'].view.setValues(nearPlane=228.325, 
    farPlane=346.015, width=240.89, height=103.3, cameraPosition=(15.1507, 
    141.212, 264.966), cameraUpVector=(-0.123867, 0.696781, -0.706508), 
    cameraTarget=(-3.05669, 10.074, -1.59871), viewOffsetX=12.8421, 
    viewOffsetY=0.273021)
session.viewports['Viewport: 1'].view.setValues(nearPlane=206.743, 
    farPlane=386.486, width=218.121, height=93.5358, cameraPosition=(172.981, 
    141.212, 210.104), cameraUpVector=(-0.529511, 0.696781, -0.483854), 
    cameraTarget=(-4.21164, 10.074, 10.1264), viewOffsetX=11.6282, 
    viewOffsetY=0.247214)
session.viewports['Viewport: 1'].view.setValues(nearPlane=212.725, 
    farPlane=380.504, width=210.966, height=90.4676, viewOffsetX=15.502, 
    viewOffsetY=0.950093)
session.viewports['Viewport: 1'].odbDisplay.setPrimaryVariable(
    variableLabel='U', outputPosition=NODAL, refinement=(INVARIANT, 
    'Magnitude'), )
mdb.save()
#: The model database has been saved to "D:\DOCTORAT\GABES Simulations\Connecting Lug\Abaqus\Lug.cae".
