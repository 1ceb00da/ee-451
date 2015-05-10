#!/bin/sh

hadoop dfs -rmr /scratch/bin

mkdir kmeans

javac -classpath /usr/usc/hadoop/1.1.2/hadoop-core-1.1.2.jar:/usr/usc/hadoop/1.1.2/lib/commons-cli-1.2.jar -d kmeans kmeans.java

hadoop dfs -mkdir /scratch/bin

jar -cvf /scratch/bin/kmeans.jar -C kmeans/  .

hadoop dfs -copyFromLocal data.txt /scratch/bin/data.txt
hadoop dfs -copyFromLocal centroid.txt /scratch/bin/centroid.txt

cd /scratch/bin

hadoop jar kmeans.jar org/myorg/kmeans /scratch/bin output

cat /scratch/bin/output2/p*

