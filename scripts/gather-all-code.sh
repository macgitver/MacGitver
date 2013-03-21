#!/bin/bash

# THIS SCRIPT SHOULD ONLY BE RUN, IF YOU KNOW WHAT YOU'RE DOING.
# It is intended to be run by a jenkins job.

# if REF is set, this ref will be used on macgitver.git and mgv-full.git
# if not, 'development' will be used. Does ONLY work with branches in mgv-full,

# expected directory structure:

# `pwd`
#  !
#  +-- mgv           -> clone of macgitver.git
#  !                    (will be updated and submodules will be init-updated)
#  !
#  +-- mgv-full.git  -> bare clone of mgv-full.git
#  !
#  +-- tmp           -> working space. Will be deleted

BASE=`pwd`
REF=${REF:-development}
TMP=$BASE/tmp
MGV=$BASE/mgv
MGV_FULL=$BASE/mgv-full.git

echo "Base      => $BASE"
echo "Ref       => $REF"
echo "Tmp       => $TMP"
echo "MGV       => $MGV"
echo "MGV-FULL  => $MGV_FULL"

# first get all the sources 

if ! [ -d $MGV ] ; then
    echo " * clone MacGitver.git"
    mkdir -p $MGV && cd $MGV
    git clone git@github.com:/macgitver/MacGitver.git .
    git checkout $REF
else
    cd $MGV
    git fetch origin
    git update-ref refs/heads/$REF origin/$REF
    git checkout -f $REF
fi

echo " * Init and update submodules"
cd $MGV
git submodule update --init --recursive
git submodule foreach --recursive 'git reset --hard && git clean -dfx'

echo " * Wipe out temporary directory"
rm -rf $TMP
mkdir $TMP

extract () {
    repo=$MGV/$1
    dest=$TMP/$1
    cd $repo
    echo " * extracting $1"
    git archive --prefix $1/ --format tar $REF | ( cd $TMP && tar xf - )
}

# reduce the files from libgit2 that we bundle to a bare minimum that is required
# legally and for the build.
cp $MGV/scripts/libgit2.export.attributes $MGV/.git/modules/libGitWrap/modules/libGitWrap/libgit2/info/attributes

extract .

for sm in $(git submodule status --recursive | awk '{ print $2 }') ; do
    extract $sm
done

# From here on, we have the destination sources collected; let's put them into 
# a git repo

if [ ! -d $MGV_FULL ] ; then
    echo " * cloning mgv-full"
    mkdir -p $MGV_FULL && cd $MGV_FULL
    git clone --bare git@babbelbox.org:/macgitver/mgv-full.git .
else
    echo " * fetching mgv-full"
    cd $MGV_FULL
    git fetch
fi

# Gather all those loose files into an INDEX
rm -f $BASE/temp.index
GIT_DIR=$MGV_FULL GIT_WORK_TREE=$TMP GIT_INDEX_FILE=$BASE/temp.index git add -A

# And write trees for that index
TREE=$(GIT_DIR=$MGV_FULL GIT_WORK_TREE=$TMP GIT_INDEX_FILE=$BASE/temp.index git write-tree)

# If we have a parent, chain it in
PARENT=$(GIT_DIR=$MGV_FULL git rev-parse --verify $REF 2>/dev/null)
PARENT=${PARENT:+-p $PARENT}

# Make a commit object for the tree object
COMMIT=$(GIT_DIR=$MGV_FULL echo "Update mgv-full.git" | git commit-tree $PARENT $TREE)

# Update the ref, so we know next time where we were comming from
GIT_DIR=$MGV_FULL git update-ref refs/heads/$REF $COMMIT

# now, push it back. This will trigger a test-build of the mgv-full source code and
# if successfull, this build will in turn trigger the second packaging script:
# create-balls.sh
GIT_DIR=$MGV_FULL git push origin $REF:$REF
