i1 = untitled(:,15) == 1;
i2 = untitled(:,15) == 0;
mri = untitled(i1,:);
no_mri = untitled(i2,:);

mri = mri(1:round(end/4),:);
no_mri = no_mri(1:round(end/4),:);

subplot(1,2,1);

scatter3(mri(:,13),mri(:,14),mri(:,16),15,mri(:,16));
title 'mri'
xlabel 'target pixel num'
ylabel 'random box num'
zlabel 'time(ms)'

subplot(1,2,2);

scatter3(no_mri(:,13),no_mri(:,14),no_mri(:,16),15,no_mri(:,16));
title 'nomri'
xlabel 'target pixel num'
ylabel 'random box num'
zlabel 'time(ms)'

subplot(1,2,1);

hist(mri(:,16),50);
title 'mri histogram'
ylabel 'count'
xlabel 'time(ms)'

subplot(1,2,2);

hist(no_mri(:,16),50);
title 'no mri histogram'
ylabel 'count'
xlabel 'time(ms)'
