
INCLUDE_DIRECTORIES(
    ${GITWRAP_INCLUDE_DIRS}
    ${HEAVEN_INCLUDE_DIRS}
    ${MACGITVER_LIBS_SOURCE_DIR}
)

ADD_SUBDIRECTORY(History)
ADD_SUBDIRECTORY(GitConfig)
ADD_SUBDIRECTORY(RefsViews)
ADD_SUBDIRECTORY(Repository)
ADD_SUBDIRECTORY(Remotes)
ADD_SUBDIRECTORY(Welcome)
ADD_SUBDIRECTORY(WorkingTree)
ADD_SUBDIRECTORY(Logging)
ADD_SUBDIRECTORY(RepoManLogger)

# Disabled to not loose the code. The code makes use of deprecated stuff and
# won't be compiling.
#
# ADD_SUBDIRECTORY( Submodules )
