# Development methods

## Versioning

The software version number conforms to [SemVer semantic 2.0.0](https://semver.org/), aka `v<MAJOR>.<MINOR>.<PATCH>`

Suffixes `alpha` or `beta` are not used

## Workflow

This project conforms to the usual **gitflow** workflow as seen in ![gitflow image](https://i.stack.imgur.com/QxVmJ.png)

Report to Vincent Driessen page [a-successful-git-branching-model](http://nvie.com/posts/a-successful-git-branching-model/) for a detailed explanation of this workflow

Branches are named as follows:

|branch name| description |
|-|-|
|`master`|usual main git branch, always holds the last software version and its tag plus documents not changing the sw version |
|`hotfix-vx.y.z`|hotfixes branches|
|`release-vx.y.0`|releases branches, patch number should always be 0 here|
|`develop`|dev branch|
|`any name`|feature branches|
