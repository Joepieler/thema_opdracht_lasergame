.PHONY: clean All

All:
	@echo "----------Building project:[ Lasergame -  ]----------"
	@cd "Lasergame" && "$(MAKE)" -f  "Lasergame.mk"
clean:
	@echo "----------Cleaning project:[ Lasergame -  ]----------"
	@cd "Lasergame" && "$(MAKE)" -f  "Lasergame.mk" clean
