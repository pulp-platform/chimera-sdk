# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>

import os
import sys

sys.path.insert(0, os.path.abspath('../../'))

project = 'Chimera SDK'
copyright = '2024, Philip Wiese, Moritz Scherer, Viviane Potocnik'
author = 'Philip Wiese, Moritz Scherer, Viviane Potocnik'
release = '2024'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'myst_parser',  # Parser for markdown files
    'breathe',  # Breathe extension for Doxygen
    'sphinxcontrib.moderncmakedomain',  # Modern CMake domain
    'sphinx_rtd_theme',  # ReadTheDocs theme
    'sphinx.ext.intersphinx',  # Link to other projects
    'sphinx.ext.todo',  # Support for todo items
    'sphinx.ext.autosectionlabel',  # Automatic section labels
    # Python Extensions
    # 'sphinx.ext.napoleon', # Support for NumPy and Google style docstrings
    # 'sphinx.ext.autodoc',
    # 'sphinx.ext.autosummary',
]
autosummary_generate = True
napoleon_use_ivar = True
add_module_names = True
autodoc_member_order = "bysource"

templates_path = ['templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', "*flycheck_*"]

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['static']

# -- Options for Breathe -----------------------------------------------------
# https://breathe.readthedocs.io/en/latest/

breathe_projects = {
    "chimera_ll": "../_build_doxygen_ll/xml",
    "chimera_hal": "../_build_doxygen_hal/xml",
}

breathe_default_project = "chimera_ll"

# -- Options for ToDo ---------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/extensions/todo.html

todo_include_todos = True

# -- Options for HTML templates ------------------------------------------------

# Extract branch name from git
branch = os.popen("git rev-parse --abbrev-ref HEAD").read().strip()

html_context = {
    'current_version':
    f"{branch}",
    'versions':
    [["master", "https://pulp-platform.github.io/chimera-sdk/"],
     ["devel", "https://pulp-platform.github.io/chimera-sdk/branch/devel/"]],
}
