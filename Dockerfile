from debian
#gross! the image ends up so big. Unfortunately I struggled with
#platformio on alpine, some kind of PATH issue where it couldn't find
#the compilers it installed. ~MM

RUN apt-get update && apt-get install -y python3 python3-pip 
WORKDIR /app
ADD platformio.ini /app/platformio.ini
RUN pip install platformio && pio pkg install

VOLUME /app 


#One can even flash the firmware from within the container if you run
# it like this:
#sudo docker build . -t ambebuilder
#sudo docker run --rm -it -v "${PWD}"/:/app --privileged -v /dev/bus/usb:/dev/bus/usb ambebuilder
#and then run `pio run -t upload` and it should 'just work' if you're on
#a linux anything like mine. Don't expect it to work from within docker
#on a non-linux, because non-linuxes often actually run a linux virtual
#machine that then runs docker. But you can still build and then flash
#with esptool (see README).

