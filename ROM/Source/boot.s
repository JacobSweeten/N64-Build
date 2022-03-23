# Copy game and run

copy_game:
	# Select the address of the ROM size
	lui		$9, 0xB000
	ori		$9, $9, 0x1000

	# Get the rom size
	lw		$14, 0($9)

	# Subtract header and boot code
	addiu	$14, $14, -0x1008

	# Select the base address of the ROM data
	addiu	$9, $9, 0x8

	# Select the base address of the destination
	lui		$10, 0x8000
	ori		$10, $10, 0x0400

	# Set length counter
	addu	$11, $0, $14
	
_copy_game_loop:
	lw		$13, 0($9)		# Read
	sw		$13, 0($10)		# Write

	# Increment by 4 bytes (1 word)
	addiu	$9, $9, 4
	addiu	$10, $10, 4

	# Decrement counter
	addi	$11, $11, -1
	bgez	$11, _copy_game_loop
	nop

finish:
	lui		$9, 0x8000		# Select entry point
	ori		$9, $9, 0x0400
	jr		$9				# Go!

