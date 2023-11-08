#!/usr/bin/bash
find . | grep .ipynb |  xargs -n1  jupyter nbconvert --ClearOutputPreprocessor.enabled=True --clear-output