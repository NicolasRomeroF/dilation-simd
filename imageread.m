f = fopen('14SIMD.raw','r');
s = fread(f,'int'); 
fclose(f);
root = sqrt(length(s))
s = reshape(s,root,root);
s = s';
imagesc(s); axis('square');colormap(gray);axis('off');