#! /bin/sh

# global variables

MAIN_DIR=$HOME/Projects/MacGitver-Release
INSTALL_DIR=$MAIN_DIR/installation


# function definitions

hr() {
    echo "----------------------------------------------------------"
}

waitEnter() {
    while true; do
        read -p "Press <Enter> to continue or <Ctrl+C> to abort ..." KEY
        if [ -z $KEY ] ; then
            echo "" ; break
        else
            echo "Please press <Enter> to continue."
        fi
    done
}

askContinue() {
while true; do
    read -p "Continue (y/n)?" yn
    case $yn in
        [Yy]* ) break;;
        [Nn]* ) exit 1;;
        * ) echo "Please answer (y)es or (n)no.";;
    esac
done
}

getGitRepo() {
    GIT_URL=$1
    DEST_DIR=$2
    if [ -d $DEST_DIR ] ; then
        echo "Directory $DEST_DIR already exists. Doing nothing."
    else
        git clone --single-branch --depth=2 $GIT_URL $DEST_DIR
        cd $DEST_DIR
        git submodule update --init --recursive --depth=2
    fi

    hr
}

installRadTools() {
    SRC_DIR=$MAIN_DIR/RAD-Tools
    BUILD_DIR=$MAIN_DIR/RAD-Tools-build

    if [ ! -d $BUILD_DIR ] ; then
        mkdir $BUILD_DIR
    fi
    cd $BUILD_DIR
    echo "Installing RAD-Tools to $INSTALL_DIR" && sleep 2
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -- $SRC_DIR && make -s install
}

makeInstall() {
    SRC_DIR="$MAIN_DIR/$1"
    BUILD_DIR="$SRC_DIR-build"

    if [ ! -d $BUILD_DIR ] ; then
        mkdir -p $BUILD_DIR
    fi

    cd $BUILD_DIR
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_PREFIX_PATH=$INSTALL_DIR -- $SRC_DIR &&
    make -s && make install
}

enterQtDir() {
    echo "Please enter the path to Qt and press <Enter>." &&
    echo "    Example: $HOME/Qt/5.5/gcc_64"

    while true; do
        read -p "Enter Path: " input
        if [ -d $input ] && [ ! -z $input ] ; then
            QTDIR=$input
            break
        else
            echo "Sorry, path not found. Try again ..."
        fi
    done
}

echo "All Project related files will be located in $MAIN_DIR"
askContinue

# Qt directory needs to be set manually
enterQtDir $QTDIR
PATH=$QTDIR/bin:$PATH

if [ ! -d $MAIN_DIR ] ; then mkdir -p $MAIN_DIR ; fi
cd $MAIN_DIR

hr
getGitRepo https://github.com/cunz-rad/RAD-Tools.git      $MAIN_DIR/RAD-Tools
getGitRepo https://github.com/macgitver/libGitWrap.git    $MAIN_DIR/libGitWrap
getGitRepo https://github.com/macgitver/libHeaven.git     $MAIN_DIR/libHeaven
getGitRepo https://github.com/macgitver/MacGitver.git     $MAIN_DIR/MacGitver

installRadTools

makeInstall libGitWrap
makeInstall libHeaven
makeInstall MacGitver

hr
echo "DONE!"
echo "Start MacGiver from here: $INSTALL_DIR/bin/MacGitver"

exit 0
