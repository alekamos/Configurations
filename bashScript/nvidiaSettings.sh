#!/bin/sh
#Shell per evitare il tearing su monitor in linux mint
nvidia-settings --assign CurrentMetaMode="$(xrandr | sed -nr '/(\S+) connected (primary )?[0-9]+x[0-9]+(\+\S+).*/{ s//\1: nvidia-auto-select \3 { ForceCompositionPipeline = On }, /; H }; ${ g; s/\n//g; s/, $//; p }')"
