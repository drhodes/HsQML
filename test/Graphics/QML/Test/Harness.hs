module Graphics.QML.Test.Harness where

import Graphics.QML.Test.Framework

import Test.QuickCheck
import Test.QuickCheck.Monadic

import Graphics.QML
import Data.IORef
import Data.Proxy
import Data.Maybe
import System.IO
import System.Directory

qmlPrelude :: String
qmlPrelude = unlines [
    "import Qt 4.7",
    "Rectangle {",
    "    id: page;",
    "    width: 100; height: 100;",
    "    color: 'green';",
    "    Component.onCompleted: {"]

qmlPostscript :: String
qmlPostscript = unlines [
    "        window.close();",
    "    }",
    "}"]

runTest :: (TestAction a) => TestBoxSrc a -> IO TestStatus
runTest src = do
    let js = showTestCode (srcTestBoxes src) ""
    tmpDir <- getTemporaryDirectory
    (qmlPath, hndl) <- openTempFile tmpDir "test1-.qml"
    hPutStr hndl (qmlPrelude ++ js ++ qmlPostscript)
    hClose hndl
    mock <- mockFromSrc src
    go <- newObject mock
    createEngine defaultEngineConfig {
        initialURL = filePathToURI qmlPath,
        contextObject = Just go}
    runEngines
    removeFile qmlPath
    status <- readIORef (mockStatus mock)
    if isJust $ testFault status
        then putStrLn $ show status
        else return ()
    return status

testProperty :: (TestAction a) => TestBoxSrc a -> Property
testProperty src = monadicIO $ do
    status <- run $ runTest src
    assert $ isNothing $ testFault status
    return ()

checkProperty :: TestType -> IO ()
checkProperty (TestType pxy) =
    quickCheck $ testProperty . constrainSrc pxy

constrainSrc :: (TestAction a) => Proxy a -> TestBoxSrc a -> TestBoxSrc a
constrainSrc = flip const
