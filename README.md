This uses [dlib](http://dlib.net/) and its implementation of the Felzenszwalb's version of the Histogram of Oriented Gradients (HoG) detector to detect traffic signs.

## Mark signs on images
a. Compile `imglab`:

```
cd dlib/tools/imglab
mkdir build
cd build
cmake ..
cmake --build .
```
_or just:_
```
./compile_imglab.sh
```

b. Create XML from sample images:

```
dlib/tools/imglab/build/imglab -c images/proibido_estacionar/training.xml images/proibido_estacionar/train/*.*
dlib/tools/imglab/build/imglab -c images/proibido_estacionar/testing.xml images/proibido_estacionar/test/*.*
```

c. Use `shift+click` to draw a box around signs and save

```
dlib/tools/imglab/build/imglab images/proibido_estacionar/training.xml
dlib/tools/imglab/build/imglab images/proibido_estacionar/testing.xml
```

## Build

```
mkdir build
cd build
cmake .. 
cmake --build .
```
_or just:_
```
./build.sh
```


## Train the fHOG detector

To train a fHOG detector on the `image/proibido_estacionar/` folder in the verbose mode, upsampling each image once, with a threshold filter with value `0.4`, execute the following command: 

```
build/hog_detector -v -u1 --filter 0.4 images/proibido_estacionar/ --detector-name svm_detectors/proibido_estacionar_detector.svm
```

Run `build/hog_detector -h` for more details.

## Visualizing HOG detectors

To visualize detectors, use the program `view_hog`:

```
build/view_hog svm_detectors/proibido_estacionar_detector.svm
```

## Examples

To run the examples:

```
build/detect --wait -u1 examples/images/*
```