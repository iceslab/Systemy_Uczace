resultNames <- c("CLASSIC_glass_test.txt", "CLASSIC_car_test.txt", "CLASSIC_vertrebal_column_test.txt");
setwd("C:/Users/Bartosz/Source/Repos/Systemy_Uczace/Systemy_Uczace/NaiveBayesTests/results")
parameters <- c("accuracy", "precision", "recall", "fscore");

for (i in resultNames) {
    dataTable = read.table(i, TRUE)

    for (j in parameters) {
        x <- c(as.numeric(unlist(dataTable["class"])))
        y <- c(as.numeric(unlist(dataTable[j])))

        name <- paste(i, j, sep = "_");
        name <- paste(name, ".png", sep = "");
        png(filename = name);
        plot(x, y, xlab = "class", ylab = j, main = i);
        dev.off();
    }
}


