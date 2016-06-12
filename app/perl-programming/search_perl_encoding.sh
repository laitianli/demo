#!/bin/bash

#查询perl支持的编码
perl -MEncode -le "print for Encode->encodings(':all')"; 
