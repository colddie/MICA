#!/bin/sh

if [ $# -eq 0 ]; then
  echo "Usage: `basename $0` filename"
  echo "Prints voxel-sizes in order z y x"
  exit 1
fi

xsize=128;
ysize=128;
zsize=47;
xscale=2.397;
yscale=2.397;
zscale=3.27
xoffset=-153.408;
yoffset=-153.408;
zoffset=0;
for argument in "$@"
do
 
basename0=`basename ${argument} .hdr`
#filename=$argument
sed '2,39d' $argument >temp

sed '/!INTERFILE/a\
name of data file := '$basename0'.v\n!GENERAL DATA :=\n!GENERAL IMAGE DATA :=\n!type of data := PET\nimagedata byte order :=\n!PET STUDY (General) :=\n!PET data type := Image\nprocess status := Reconstructed\n!number format := float\n!number of bytes per pixel := 4\nnumber of dimensions := 3\n\nmatrix axis label [1] := x\n!matrix size [1] := '$xsize'\nscaling factor (mm/pixel) [1] := '$xscale'\nmatrix axis label [2] := y\n!matrix size [2] := '$ysize'\nscaling factor (mm/pixel) [2] := '$yscale'\nmatrix axis label [3] := z\n!matrix size [3] := '$zsize'\nscaling factor (mm/pixel) [3] :='$zscale'\n;origin (in mm):={0,0,0}\nfirst pixel offset (mm) [1] := '$xoffset'\nfirst pixel offset (mm) [2] := '$yoffset'\nfirst pixel offset (mm) [3] := '$zoffset'\n\nnumber of time frames := 1\nimage scaling factor[1] := 1\ndata offset in bytes[1] := 0\nquantification units := 1' temp >$basename0.hv
rm temp

cp $basename0.img $basename0.v 
done 
























