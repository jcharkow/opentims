% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/opentimsr.R
\name{retention_times}
\alias{retention_times}
\title{Get the retention time for each frame.}
\usage{
retention_times(opentims)
}
\arguments{
\item{opentims}{Instance of OpenTIMS.}
}
\value{
Retention times corresponding to each frame.
}
\description{
Get the retention time for each frame.
}
\examples{
\dontrun{
D = OpenTIMS('path/to/your/folder.d')
print(retention_times(D)) 
}
}
