


k=1024;
fid=fopen('output.raw','rb');
GY=fread(fid,[k*k],'uint8');
m=reshape(GY,k,k);
m=m';
m1=m./255;
figure(2)
imshow(m1);








