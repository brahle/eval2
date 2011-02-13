#/usr/bin/env python
# Copyright 2011 Bruno Rahle
#
# TODO: Paste copy statement
# TODO: Documentation cleanup

from abc import ABCMeta, abstractmethod

class Field(object):
    """An abstract class that holds basic methods every Field should implement.
    """
    __metaclass__ = ABCMeta

    def __init__(self, field):
        self._field = field
        self.prepare()
        if not self.verify():
            raise TypeError(str(self.__class__) + ' has wrong type of data')

    def get_value(self):
        """This function returns the value of the field."""
        return self._field

    def prepare(self):
        """This method prepares the field. It should do all the required
        parsing and modifying with the field. """
        pass

    @abstractmethod
    def __str__(self):
        """This function just invokes the inherited __str__ function. It should
        be overloaded to suit the need of a particular Field."""
        super.__str__(self)

    @abstractmethod
    def verify(self):
        """This function, when implemented, should verify that the Field is of
        the required type and that the information it holds is plausible."""
        pass

    def nonempty(self):
        """Checks if the field is nonempty. You might want to overload this."""
        return self._field is not None

class NumberField(Field):
    """An abstract class that is base class for all number types in Python"""
    def __str__(self):
        """A NumberField just prints the number it holds."""
        return str(self._field)

class IntegerField(NumberField):
    """A class that holds all integer types."""
    def prepare(self):
        """If the given type is a string, it tries to convert it to an integer
        """
        if isinstance(self._field, str):
            self._field = int(self._field)

    def verify(self):
        """Checks if the type is integer."""
        return isinstance(self._field, int)


class FloatField(NumberField):
    """A class that holds all floating point types."""
    def prepare(self):
        """If the given type is a string, it tries to convert it to an float
        """
        if isinstance(self._field, str):
            self._field = float(self._field)

    def verify(self):
        """Checks if the type is Float."""
        return isinstance(self._field, float)


class StringField(Field):
    """A class that holds all string types."""
    def __str__(self):
        """Prints the value of the string formatted appropriately."""
        return '"' + self._field + '"'

    def prepare(self):
        """Strips the unwanted whitespace from the end of the string."""
        self._field = self._field.rstrip()

    def verify(self):
        """Verifies that the field holds a string."""
        return isinstance(self._field, str)

    def nonempty(self):
        """Checks if the string is nonempty."""
        return self._field is not None and len(self._field) > 0


class ListField(Field):
    """A class that is used to hold a list. It should be a list of strings
    and none of those strings may have newline characters """
    def __str__(self):
        """Prints the comma-separeted list with every element surrounded with
        quotation marks."""
        return ','.join('"' + x + '"' for x in self._field)

    def prepare(self):
        """Prepares the list to be used. If it is given as a string, it tries
        to parse it. It also strips all the whitespace from the begining and
        ending of all strings."""
        if isinstance(self._field, str):
            parts = self._field.split(',')
            self._field = parts
        for i in range(len(self._field)):
            if isinstance(self._field[i], str):
                self._field[i] = self._field[i].strip()
        self._field = list(filter(None, self._field))

    def verify(self):
        """Verifies that the field holds a list, and that each string in the
        list holds no newline characters."""
        if not isinstance(self._field, list):
            return False
        for part in self._field:
            if part.find('\n') != -1:
                raise TypeError(str(self.__class__) +
                                ' cannot hold a string with newlines')
        return True

    def nonempty(self):
        """Checks if the list is nonempty."""
        return self._field is not None and len(self._field) > 0


def main():
    pass

if __name__ == '__main__':
    main()

