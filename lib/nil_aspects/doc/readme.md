# Nil Aspects

Generally speaking Aspects will take the data contained in nil and convert
it to something useable for that domain.

## Design Goals

The goal of each aspect is to translate the data into something useable by
some first or thirdparty these can be considered controllers to these libs.

These aspect should rely on Nil to provide threading etc. Since it will be
Nil that decides on platform specifics.
