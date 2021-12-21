file=$PWD/example.g3a
cd /media/sid/disk && \
rm example.g3a
cp "$file" ./ && \
cd ../ && \
umount /dev/sda1
sudo eject /dev/sda
