# ----------------------------------------------------------------------
# 
# File: conf.py
# 
# Last edited: 17.12.2024
# 
# Copyright (C) 2024, ETH Zurich and University of Bologna.
# 
# Authors:
# - Philip Wiese (wiesep@iis.ee.ethz.ch), ETH Zurich
# 
# ----------------------------------------------------------------------
# SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


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
    'myst_parser', # Parser for markdown files
    'breathe',     # Breathe extension for Doxygen
    'sphinxcontrib.moderncmakedomain', # Modern CMake domain
    'sphinx_rtd_theme', # ReadTheDocs theme
    'sphinx.ext.intersphinx', # Link to other projects
    'sphinx.ext.todo', # Support for todo items
    'sphinx.ext.autosectionlabel', # Automatic section labels
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
