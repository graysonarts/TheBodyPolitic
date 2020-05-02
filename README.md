# The Body Politic

_TODO: Insert video documentation_

## Artist Statement

This installation is a meditation on COVID-19 and the inherit disfunction of the political systems that hide, denied,
and otherwise, provided false information at the expense of human lives. The use of the human form as the canvas for
the projection is to highlight the human costs of the machinations of our leaders.

Each particle represents a country in the world, and the size of each particle is the number of confirmed cases
over time, derived from global reports as collected by
[Tableau's Covid-19 Data Hub](https://www.tableau.com/covid-19-coronavirus-data-resources). It is a first collaboration
between Russell "Grayson" Hay and Kai Curtis.

## Artist Bios

### Russell "Grayson" Hay

Russell was born in southern Louisiana, and has explored various aspects of creativity and creation throughout his
life. The body of his work focuses on movement of the human body either in still images, video productions, or
live performances. When he is not actively creating art, he toils away at a computer for Tableau Software.

### Kai Curtis

_TODO: Add Bio_

## Technical Details

### Basic Information

This installation is an openframeworks application with ofxPiMapper handling the wrapping for the projection mapping.
The mannequin was purchased from an architectural salvage store, and the outer layer of paint was stripped off painstakingly
with a heatgun, scrapper, and lots of time. The images are projected with an Optoma DLP Project mounted on a microphone stand
with a Manfrotto superclamp.

We load the data downloaded from [data.world](https://data.world/) and store it locally. We then bucket and aggregate along
the time and country dimensions.

Colors are chosen from images, many of which were cultivated from [uigradients.com](https://uigradients.com), and then
modified to make "seamless" using Adobe Photoshop.

The source code is available on github [~RussTheAerialist/TheBodyPolitic](https://github.com/RussTheAerialist/TheBodyPolitic).
The full documentation post for this piece can be found at [graysonarts/post/the_body_politic]()

### Addons Required

* [https://github.com/kr15h/ofxPiMapper.git](ofxPiMapper)
* [https://github.com/paulvollmer/ofxCsv.git](ofxCsv)
* [https://github.com/bakercp/ofxTime.git](ofxTime)
* [https://github.com/danomatika/ofxMidi.git](ofxMidi)
