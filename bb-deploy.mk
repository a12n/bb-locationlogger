DEV ?= 169.254.0.1
PGK ?= arm/o.le-v7/locationlogger.so.bar


.PHONY: deploy install launch terminate uninstall


install: CMD=-installApp
install: deploy

launch: CMD=-launchApp
launch: deploy

terminate: CMD=-terminateApp
terminate: deploy

uninstall: CMD=-uninstallApp
uninstall: deploy


deploy:
	blackberry-deploy ${CMD}	\
		-device ${DEV} -password ${PASS}	\
		-package ${PKG}
