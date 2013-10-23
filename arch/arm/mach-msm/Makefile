obj-y += timer.o
obj-y += clock.o

obj-$(CONFIG_MSM_SCM) += scm.o scm-boot.o

CFLAGS_scm.o :=$(call as-instr,.arch_extension sec,-DREQUIRES_SEC=1)

obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o
obj-$(CONFIG_SMP) += headsmp.o platsmp.o

obj-$(CONFIG_ARCH_MSM_DT) += board-dt.o
