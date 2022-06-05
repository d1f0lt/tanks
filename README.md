# Just Tanks

Just tanks is a free and opensource multiplayer arcade game inspired by "Battle City". Your goal is to destroy the opponent's tanks and not be destroyed
by them. You can choose either a multiplayer to play with up to 10 friends or single-player to improve your
skills with relaxing game against bots. Get money for your wins and use them to transform your little tank into
the war machine.

## Controls

* W &ndash; move up
* A &ndash; move left
* S &ndash; move down
* D &ndash; move right
* SPACE &ndash; make a shot
* ESC &ndash; to pause game

You can also use the arrow keys on your keyboard to move.

## How to build

Note: this project supports only Linux and gcc compiler. But feel free to try build on other systems.

### Requirements

* libsqlite3
* sfml
* boost-asio
* cmake
* make
* g++ version at least 10

### Build instruction

    git clone https://github.com/d1f0lt/tanks
    cd tanks
    cmake .
    make 

## How to play

     cd bin
    ./tanks # to play game
    ./tanks server <level number> <players> <bots> <bonues> # to run a server

## LICENSE

Just Tanks is licensed under the 2-Clause BSD License. See LICENCE for details.
