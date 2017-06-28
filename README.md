# LGML

## Documentation

### Site statique

- [clairezed.github.io/LGML_docs](https://clairezed.github.io/LGML_docs/)
- [lgmldocs.netlify.com](http://lgmldocs.netlify.com/) (permet d'accéder à l'interface admin)
- [Accès admin ](http://lgmldocs.netlify.com/admin)

### Wiki


### Gitbook

## Theme

Themes possibles :
- https://html5up.net/editorial
- http://idratherbewriting.com/documentation-theme-jekyll/
- https://rimeofficial.github.io/jelly-bean/
- https://learn.cloudcannon.com/templates/edition/
- https://learn.cloudcannon.com/templates/base/


## Configuration

La documentation réside dans un folder /docs.

Pour ajouter un CMS avec Netlify CMS dans cette configuration, voici les settings à renseigner dans l'[interface de netlify](https://app.netlify.com/sites/lgmldocs/settings) :

| Repository        | https://github.com/clairezed/LGML_docs |
| Branch            | master                                 |
| Build command     |./build_docs.sh                         |
| Publish directory | docs/_site                             |
