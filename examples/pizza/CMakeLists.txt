set(PIZZA_HEADERS Pizza.h Topping.h toppings/Cheese.h toppings/Mozzarella.h toppings/Champignon.h toppings/TomatoSauce.h BakedEvent.h)
set(PIZZA_SOURCES toppings/Cheese.cpp Topping.cpp toppings/Mozzarella.cpp Pizza.cpp toppings/Champignon.cpp toppings/TomatoSauce.cpp)

add_executable(pizza main.cpp ${PIZZA_HEADERS} ${PIZZA_SOURCES})
target_link_libraries(pizza polypropylene)