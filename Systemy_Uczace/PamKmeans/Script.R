require(foreign)
require(cluster)
require(clusterCrit)

purity <- function(clusters, classes) {
    return(sum(apply(table(classes, clusters), 2, max)) / length(clusters))
}

getKmeansStats <- function(inData, columns, clusters, algorithm) {
    result <- kmeans(data.matrix(inData[, 1:columns]), clusters, algorithm = algorithm)
    stats <- intCriteria(data.matrix(inData[, 1:columns]), result$cluster, c("Davies_Bouldin", "Silhouette", "Dunn"))
    stats[["purity"]] <- purity(result$cluster, data.matrix(inData[length(inData)]))

    return(stats)
}

getPamStats <- function(inData, columns, clusters, algorithm) {
    result <- pam(data.matrix(inData[, 1:columns]), clusters, metric = algorithm)
    stats <- intCriteria(data.matrix(inData[, 1:columns]), result$cluster, c("Davies_Bouldin", "Silhouette", "Dunn"))
    stats[["purity"]] <- purity(result$cluster, data.matrix(inData[length(inData)]))
    return(stats)
}

crossValidationKmeans <- function(inData, columns, clusters, algorithm) {
    results <- NULL
    retVal <- NULL
    for (i in 1:10) {
        # These indices indicate the interval of the test set
        frame <- cbind(inData)
        train <- NULL
        indices <- (((i - 1) * round((1 / 10) * nrow(frame))) + 1):((i * round((1 / 10) * nrow(frame))))
        # Exclude them from the train set
        train <- rbind(train, frame[-indices,])
        rownames(train) <- NULL

        results <- rbind(results, getKmeansStats(train, columns, clusters, algorithm))
    }

    retVal[["davies_bouldin"]] <- mean(as.double(results[, "davies_bouldin"]))
    retVal[["silhouette"]] <- mean(as.double(results[, "silhouette"]))
    retVal[["dunn"]] <- mean(as.double(results[, "dunn"]))
    retVal[["purity"]] <- mean(as.double(results[, "purity"]))
    

    return(retVal)
}

crossValidationPam <- function(inData, columns, clusters, algorithm) {
    results <- NULL
    retVal <- NULL
    for (i in 1:10) {
        # These indices indicate the interval of the test set
        frame <- cbind(inData)
        train <- NULL
        indices <- (((i - 1) * round((1 / 10) * nrow(frame))) + 1):((i * round((1 / 10) * nrow(frame))))
        # Exclude them from the train set
        train <- rbind(train, frame[-indices,])
        rownames(train) <- NULL

        results <- rbind(results, getPamStats(train, columns, clusters, algorithm))
    }

    retVal[["davies_bouldin"]] <- mean(as.double(results[, "davies_bouldin"]))
    retVal[["silhouette"]] <- mean(as.double(results[, "silhouette"]))
    retVal[["dunn"]] <- mean(as.double(results[, "dunn"]))
    retVal[["purity"]] <- mean(as.double(results[, "purity"]))


    return(retVal)
}

setwd("D:/Repos/Systemy_Uczace/Systemy_Uczace/PamKmeans")
print(sprintf("Working directory: %s", getwd()))

paths <- c("dane/seeds_test.arff", 
           "dane/glass_test.arff", 
           "dane/vertrebal_column_test.arff", 
           "dane/user_knowledge_test.arff")

resultRows <- NULL

for (path in paths) {
    print(sprintf("Processing: %s", path))
    inData <- read.arff(path)

    for (algorithm in 1:2) {
        #print(sprintf("algorithm: %1.0f", algorithm))
        # Column count
        for (columns in 1:(length(inData) - 1)) {
            #print(sprintf("columns: %1.0f", columns))
            # Cluster count
            for (clusters in 2:10) {
                print(sprintf("algorithm: %1.0f columns: %1.0f clusters: %1.0f", algorithm, columns, clusters))
                # Kmeans
                if (algorithm == 1) {
                    algorithmName = "Kmeans"
                    for (algorithmParam in c("Hartigan-Wong", "Forgy", "MacQueen")) {
                        result <- crossValidationKmeans(inData, columns, clusters, algorithmParam)
                        resultRows <- rbind(resultRows, sprintf("%s,%s,%1.0f,%1.0f,%f,%f,%f,%f", path, algorithmName, columns, clusters, result["davies_bouldin"], result["silhouette"], result["dunn"], result["purity"]))
                    }
                }
                # PAM
                else {
                    algorithmName = "PAM"
                    for (algorithmParam in c("euclidean", "manhattan")) {
                        result <- crossValidationPam(inData, columns, clusters, algorithmParam)
                        resultRows <- rbind(resultRows, sprintf("%s,%s,%1.0f,%1.0f,%f,%f,%f,%f", path, algorithmName, columns, clusters, result["davies_bouldin"], result["silhouette"], result["dunn"], result["purity"]))
                    }
                }
            }
        }
    }
}

print(resultRows)
write.table(resultRows,"allData.txt")
print("Done")
