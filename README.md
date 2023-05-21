# nerv.hpp

Повторная реализация моего прототипа HAL'о-подобного нечто для микроконтроллеров 
семейства AVR с упором на реализацию безопасных zero-cost абстракций. 

Ахтунг. Этот проект создается в учебных целях и не предназначен для промышленного использования,
так как имеет нестабильный API, не отвечает стандартам качества и т.д.. В общем никаких 
гарантий не даю, смотрите [лицензию](./LICENSE). 

## Get started 

<!-- Библиотека не предполагается в качестве единицы компиляции. Также, к сожалению, разделения на интерфейс  -->
<!-- и имплантацию нету, так как мне не нужно было, но возможно появиться в будущем. -->

Для работы HAL'а требуется установленная avr-libc и поддержка C++11.

Методы установки
- Ручная установка
    ```console
    $ git clone --depth=1 https://codeberg.org/unqtd/nerv.hpp.git vendor/nerv
    ```
- [buildavr](https://github.com/unqtd/buildavr)
    ```yaml
    dependencies: 
      - name: nerv
        git: https://codeberg.org/unqtd/nerv.hpp.git
    ```

Смотрите [примеры использования](https://github.com/unqtd/homework/tree/main/%D0%9C%D0%9F%D0%A1).

## List of supported chips

|    ATtiny       |
| :-------------: |
|  `attiny2313a`  |

## Design

<!-- ## Documentation -->

<!-- Для пользователя: -->
<!-- * [Examples](https://codeberg.org/unqtd/homework/src/branch/main/%D0%9C%D0%9F%D0%A1) -->
<!-- * Handbook -->

<!-- Для разработчика: -->
<!-- * Handbook -->

## References

* [MicroCore](https://github.com/MCUdude/MicroCore)
