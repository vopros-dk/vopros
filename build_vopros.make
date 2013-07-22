; Make file for building a Vopros installation.

api = 2
core = 7.x

projects[drupal] = drupal
projects[drupal][version] = 7.22

projects[vopros][type] = "profile"
projects[vopros][download][type] = "git"
projects[vopros][download][url] = "git@github.com:DBCDK/Vopros-install.git"
