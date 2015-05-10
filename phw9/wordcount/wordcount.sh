#!/bin/sh

hadoop dfs -rmr /scratch/bin

mkdir wordcount

javac -classpath /usr/usc/hadoop/1.1.2/hadoop-core-1.1.2.jar:/usr/usc/hadoop/1.1.2/lib/commons-cli-1.2.jar -d wordcount WordCount.java

hadoop dfs -mkdir /scratch/bin

jar -cvf /scratch/bin/wordcount.jar -C wordcount/  .

hadoop dfs -copyFromLocal data.txt /scratch/bin/data.txt

cd /scratch/bin

hadoop jar wordcount.jar org/myorg/WordCount data.txt output

cat output/part-r-00000 

