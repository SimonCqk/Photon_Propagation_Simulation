# Biological Photon Propagation Simulation Analysis System

[![Snap CI branch](https://img.shields.io/snap-ci/ThoughtWorksStudios/eb_deployer/master.svg)]()
[![Packagist](https://img.shields.io/packagist/l/doctrine/orm.svg)]()

> Intro : This is a Biological Photon Propagation Simulation Analysis System Based On Monte Carlo Simulation , with a childish GUI.

<br>

|  Platfrom  | IDE  |  Compiler  |   Database   |
|:----------:|:--------:|:----------:|:----------:|
| Windows 10 | Qt Creator(5.9.1) | VC++2017 | SQLite |

### How to use

#### 1. Enter the main interface and specify the parameters for a model-running.

  ![conf](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/ConfigureParas.png?raw=true)
  
  + The parameters include:
  + Number of Photons
  + z/r Grid Speparation of two-dimensional homogeneous grid system
  + total numbers of grid elements in z/r/alpha directions.
  + Refractive Index/Absorption Coefficient/Scattering Coefficient/Anisotropy/Thickness of each layer
  + Refractive Index for medium Above/Below
  ##### after specified.
  
  ![sample](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/Sample.png?raw=true)
  
#### 2. Click the `Run` button to start a run.
#### 3. View the Run Results on `Run Results` page.

  ![runresults](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/Results.png?raw=true)

  ##### see more raw data by clicking `Data` button.
  
  ![runresults_sample](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/Results_sample.png?raw=true)

  ##### you may want to view visualized chart instead of raw data , so click `Scatter Chart` or `Spline chart` to lanuch chart viewer.
  ![runresults_sample](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/Results_scatterchart.png?raw=true)
  ![runresults_sample](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/Results_splinechart.png?raw=true)
  
#### 4. View Running History on `History` page.
  ![history](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/History.png?raw=true)
  
  Input `Running Data` and `Number of Running`,and click `Query` button to ask for query...
  (the `Number of Running` is shown in the status bar when launched.)<br>
  `You have to add .dll file of SQLite to sqldrivers folder. `<br>
  As this sample:
  
  ![query](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/History_query.png?raw=true)

#### 5. Finally there is a cute `About` page...lol...
![about](https://github.com/SimonCqk/Model_of_LightTransport/blob/master/image/About.png?raw=true)

More Feature are being prepared...lol...

