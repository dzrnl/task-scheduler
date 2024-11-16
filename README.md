# TaskScheduler

Эффективный шедулер для выполнения зависимых задач.

## Задача

Разработать класс, отвечающий за выполнение связанных по данным задач.

Часто для решения задачи требуется выполнить граф вычислений, где узел графа — это задача, а ребро — связь между
результатом выполнения одной задачи и параметром для запуска другой. Требуется разработать класс **TaskScheduler**,
решающий подобную задачу.

Пример такой задачи - нахождение корней квадратного уравнения (где гарантируется наличие двух корней).
Наивное решение подобной задачи подразумевает вычисление корня из дискриминанта дважды, что не оптимально.

Код для решения данной задачи с помощью TaskScheduler:

```cpp
float a = 1;
float b = -2;
float c = 0;

TaskScheduler scheduler;

auto& id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);

auto& id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));

auto& id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

auto& id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

auto& id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));

auto& id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

scheduler.executeAll();

std::cout << "x1 = " << scheduler.getResult<float>(id5) << std::endl;
std::cout << "x2 = " << scheduler.getResult<float>(id6) << std::endl;
```

Где ```getFutureResult``` это результат выполнения задачи в будущем.

### Публичный интерфейс TaskScheduler

- **add** - принимает в качестве аргумента задание для него. Возвращает объект описывающий добавленную таску.
- **getFutureResult<T>** - возвращает объект, из которого в будущем можно получить результат задания, переданного в
  качестве результата типа Т
- **getResult<T>** - возвращает результат выполнения задания определенного типа. Вычисляет его если оно еще не
  подсчитано
- **executeAll** - выполняет все запланированные задания

### Требования и ограничения к заданиям

- [Callable object](https://en.cppreference.com/w/cpp/named_req/Callable)
- Аргументы гарантированно [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)
- Возвращаемое значение произвольное,
  гарантированно [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)

## Ограничения

- Запрещено использовать стандартную библиотеку, за исключением контейнеров и умных указателей.

## Тесты

Все вышеуказанный класс должен быть покрыты тестами, с помощью
фреймворка [Google Test](http://google.github.io/googletest).

## NB

В данной работе могут быть использованы
идеи [Type Erasure](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure).
