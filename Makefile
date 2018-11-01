.PHONY: clean All

All:
	@echo "----------Building project:[ Laser_game - Debug ]----------"
	@cd "Laser_game" && "$(MAKE)" -f  "Laser_game.mk"
clean:
	@echo "----------Cleaning project:[ Laser_game - Debug ]----------"
	@cd "Laser_game" && "$(MAKE)" -f  "Laser_game.mk" clean
