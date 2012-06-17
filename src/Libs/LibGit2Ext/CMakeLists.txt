
# This sub-project contains extentions to libgit2.
# Thus, it's written in plain old C; These files are meant to belong to libgit2's structure
# as follows:
#
# Matching Regex	Destination
# .*[^_][^p]\.h		include/		( If files exist there already, they're meant to be additions )
# .*\_\p\.h			src/			( with `sed -e s:_p::` run on the filename )
# .*\.c				src/

PROJECT( GIT2EXT )

INCLUDE_DIRECTORIES(
	${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

	checkout.c
)

SET( HDR_FILES
	checkout.h
)

ADD_LIBRARY(
	git2ext STATIC

	${HDR_FILES}
	${SRC_FILES}
)

#TARGET_LINK_LIBRARIES(
	#git2ext
	#git2
#)
