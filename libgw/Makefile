# Makefile - libgw.dsw

.PHONY: all
all: \
	libgw \
	libgw_example \
	example_single_win \
	example_multi_wins \
	example_multi_wins_glui \
	example_multi_wins_global \
	example_single_win_global

.PHONY: libgw
libgw:
	$(MAKE) -C libgw -f libgw.mak

.PHONY: libgw_example
libgw_example:
	$(MAKE) -C libgw_example_single_win -f libgw_example_single_win.mak

.PHONY: example_single_win
example_single_win:
	$(MAKE) -C examples/example_single_win -f example_single_win.mak

.PHONY: example_multi_wins
example_multi_wins:
	$(MAKE) -C examples/example_multi_wins -f example_multi_wins.mak

.PHONY: example_multi_wins_glui
example_multi_wins_glui:
	$(MAKE) -C examples/example_multi_wins_glui -f example_multi_wins_glui.mak

.PHONY: example_multi_wins_global
example_multi_wins_global:
	$(MAKE) -C examples/example_multi_wins_global -f example_multi_wins_global.mak

.PHONY: example_single_win_global
example_single_win_global:
	$(MAKE) -C examples/example_single_win_global -f example_single_win_global.mak

.PHONY: clean
clean:
	$(MAKE) -C libgw -f libgw.mak clean
	$(MAKE) -C libgw_example_single_win -f libgw_example_single_win.mak clean
	$(MAKE) -C examples/example_single_win -f example_single_win.mak clean
	$(MAKE) -C examples/example_multi_wins -f example_multi_wins.mak clean
	$(MAKE) -C examples/example_multi_wins_glui -f example_multi_wins_glui.mak clean
	$(MAKE) -C examples/example_multi_wins_global -f example_multi_wins_global.mak clean
	$(MAKE) -C examples/example_single_win_global -f example_single_win_global.mak clean

.PHONY: depends
depends:
	$(MAKE) -C libgw -f libgw.mak depends
	$(MAKE) -C libgw_example_single_win -f libgw_example_single_win.mak depends
	$(MAKE) -C examples/example_single_win -f example_single_win.mak depends
	$(MAKE) -C examples/example_multi_wins -f example_multi_wins.mak depends
	$(MAKE) -C examples/example_multi_wins_glui -f example_multi_wins_glui.mak depends
	$(MAKE) -C examples/example_multi_wins_global -f example_multi_wins_global.mak depends
	$(MAKE) -C examples/example_single_win_global -f example_single_win_global.mak depends


