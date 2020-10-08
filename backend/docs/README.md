## Documentation

To automatically generate documentation for the backend usinf sphinx enter `make api`. This will update the `.rst` files with current changes in the library.   

```bash
(backend) $ cd docs
(docs) $ make api
```
Then to build HTML pages use `make html`.
```bash
(docs) $ make html
```
The generated HTML pages can be found in `source/_build/html` and the website can accessed using `index.html`.