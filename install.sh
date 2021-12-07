file=$PWD/project.g3a
cd /media/sid/disk && \
rm project.g3a && \
cp "$file" ./ && \
cd ../ && \
umount /dev/sda1
sudo eject /dev/sda
