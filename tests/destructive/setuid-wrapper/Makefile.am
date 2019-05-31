if HAVE_LIBLTTNG_UST_CTL
AM_CPPFLAGS += -I$(srcdir)

noinst_PROGRAMS = change_user

change_user_SOURCES = change_user.c tp.c
change_user_LDADD = -llttng-ust $(DL_LIBS)

noinst_SCRIPTS = test-setuid-wrapper
endif
