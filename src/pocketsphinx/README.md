# How to set up pocketsphinx package

1. Install `pocketsphinx` package and put the following files into repective folders. Change the content if you want to detect different keywords.
    * voice_cmd.dic & voice_cmd.kwlist\
    *src/pocketsphinx/vocab*
You need to change the model path in scripts/recognizer.py if the acoustic model is in a different location in your system, or you want to use a different language model (the default is US English).

2. Launch the node.
```
$ roslaunch pocketsphinx pocketsphinx.launch
```