function [I header]=readim(filename)
% float and int support
	disp(['	Retrieving Image ' filename ' ...']);
	fid=fopen(filename,'rb');
	if (fid==-1)
	  	error('can not open imput image filem press CTRL-C to exit \n');
	  	pause
	end

    header=fread(fid,8369,'int');
    fseek(fid,8321,'bof'); %skip the header
info = fread(fid,5,'int');
datatype = info(1);
switch datatype
    case 8
        bitsallocated = 16;     % SIGNED_SHORT, 4bytes
    case 4
        bitsallocated = 32;     % FLOAT, 8bytes
end

ndim = info(2);
switch ndim
    case 3
        nImages=info(3);       % z
        height=info(4);        % y
        width=info(5);         % x
    case 2
        height=info(3);        % y
        width=info(4);         % x
        nImages=1;             % z
end

if nImages~=1             % ndim=3
I=zeros(height,width,nImages);
for i = 1:nImages
fseek(fid,8369+height*width*4*(i-1),'bof'); % skip the header
temp = fread(fid,[height width],'float32');
temp=temp';
I(:,:,i) = temp;
% I(:,:,i)= mat2gray(temp);
end

else                     % ndim=2
I=zeros(height,width);
fseek(fid,8369,'bof');                 % skip the header
temp = fread(fid,[height width],'float32');
temp=temp';
I = temp;
end


fclose(fid);