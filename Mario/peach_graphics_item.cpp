#include "peach_graphics_item.h"
#include "element.h"

Peach_graphics_item::Peach_graphics_item(Container c, Element* e) : container_type(c), container_object(e){

}

Peach_graphics_item::~Peach_graphics_item(){
    delete container_object;
}
