# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SPHINXAPIDOC  = sphinx-apidoc
SOURCEDIR     = source
APIDIR	      = "$(SOURCEDIR)/api"
BUILDDIR      = "$(SOURCEDIR)/_build"
EXCLUDE		  = "../tests"
PROJECT	      = Pufferfish
AUTHOR	      = Pez-Globo

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Generate API stubs
api: export PYTHONPATH := ../
api:
	@rm -r "$(APIDIR)"
	@$(SPHINXAPIDOC) -f -a -M -d 5 -H "$(PROJECT)" -A "$(AUTHOR)" -o "$(APIDIR)" "../" "$(EXCLUDE)"
	@echo "Auto-generation of API documentation finished." \
		"The generated api files are in $(APIDIR)."

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
