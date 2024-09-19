# README.git

Generating releases on GitHub is a very simple process;

## Set version in repo
1) Edit the file "Makefile".  Changing the "VERSION" line to the new version.
2) Do a "git commit" just for the version number change.

## Create new release
1) Go to the [GitHub release page](https://github.com/ncroxon/gnu-efi/releases/new)
2) Enter the version number in the tag field, selecting "create tag on publish"

   **Ignore GitHub's suggestion of prefixing the version with a "v"**
3) **OPTIONAL**
   Add a release description and/or a list of pull requests and contributors via "Generate release notes"
4) For alpha/beta versions, set it to be a pre-release
5) Release the new version
6) GitHub will automatically generate a GZ compressed TAR file and a ZIP file of the repo

    


