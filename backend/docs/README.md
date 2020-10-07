## Documentation

To automatically generate documentation for the backend use sphinx as shown below:

```bash
(backend) $ sphinx-apidoc -F -a -M -H Pufferfish -A Pez-Globo -o source . tests
(backend) $ cd docs
```
Open `conf.py` change the module path, in `Path setup`, to as shown below.
```bash
sys.path.insert(0, \'path\\to\\pufferfish-vent-software\\backend\')
``` 
This will enable sphinx to detect `ventserver` package and it\'s modules. After making necessary changes to `conf.py` run `make html`.
```bash
(docs) $ make html
```
The generated HTML files would be in `docs/_build` and the website can accessed using `index.html`. 