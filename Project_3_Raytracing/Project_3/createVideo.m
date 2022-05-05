
% Create the video 
video = VideoWriter('my_video.mp4'); 
open(video); %open the file for writing

%where N is the number of images
for ii=1:360 
  path = strcat('.Project_3/video/frames/ray', string(ii), '.png');
  I = imread(path); %read the next image
  writeVideo(video,I); %write the image to file
  ii
end
close(video);