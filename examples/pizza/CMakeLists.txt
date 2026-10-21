add_library(pizzalib
        Pizza.h Topping.h toppings/Cheese.h toppings/Mozzarella.h toppings/Champignon.h toppings/TomatoSauce.h BakedEvent.h
        toppings/Cheese.cpp Topping.cpp toppings/Mozzarella.cpp Pizza.cpp toppings/Champignon.cpp toppings/TomatoSauce.cpp)
target_include_directories(pizzalib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pizzalib PUBLIC Polypropylene::polypropylene)

add_executable(pizza main.cpp)
target_link_libraries(pizza PRIVATE pizzalib)
