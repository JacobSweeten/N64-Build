all: Utils ROM FORCE

Utils: MakeCart PackAssets FORCE

MakeCart: Common FORCE
	cd Utils/MakeCart && make

PackAssets: Common FORCE
	cd Utils/PackAssets && make

Common: FORCE
	cd Utils/Common && make

ROM: FORCE
	cd ROM && make

FORCE: ;