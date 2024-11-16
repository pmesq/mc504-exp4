sudo docker rm -f $(sudo docker ps -aq)
sudo docker build -t exp4 .
sudo docker run -it --rm exp4
