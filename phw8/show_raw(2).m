k=194;
fid=fopen('phw8.raw','rb');
GY=fread(fid,[k*k],'uint8');
fclose(fid);



m=reshape(GY,k,k);
%m=m';
m1=m./255;
figure(1)
imshow(m1);

