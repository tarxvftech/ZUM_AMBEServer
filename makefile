docker: 
	sudo docker run --rm -it -v "${PWD}"/:/app --privileged -v /dev/bus/usb:/dev/bus/usb ambebuilder
img:
	sudo docker build . -t ambebuilder
