if [ -n "`ps aux | grep IMSLnewsspider | grep -v grep `" ];
then echo "the pidname exist";
else nohup /disc1/usr/daming.yang/ydmdir/IMSLspiderrunin41news/IMSLnewsspider -start &
fi
