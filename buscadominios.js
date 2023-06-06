const dns = require('dns');
const fs = require('fs');
// Obter o domínio principal do primeiro argumento
const dominio = process.argv[2];
const listSubDom = process.argv[3];
// Verificar se o argumento do domínio principal foi fornecido
if (!dominio || !listSubDom) {
  console.log("Criado por Gabriel Logan")
  console.log("Modo de uso: node nome_do_arquivo.js <Dominio principal> <Lista com sub dominios>");
  console.log("Exemplo: node nome_do_arquivo.js businesscorp.com.br subdominios.txt");
  process.exit(1);
}
fs.readFile(listSubDom, 'utf8', (err, data) => {
  if (err) {
    console.error(`Erro ao abrir o arquivo '${listSubDom}'. Certifique-se de que ele existe no mesmo diretório do executável.`);
    return;
  }
  const subdominios = data.split('\n');
  const totalPalavras = subdominios.length;
  let palavrasLidas = 0;
  let quantiaHostEncontrado = 0;
  subdominios.forEach((subdominio) => {
    subdominio = subdominio.trim();
    if (subdominio === "") {
      return;
    }
    const hostname = `${subdominio}.${dominio}`;
    dns.lookup(hostname, (err, address) => {
      if (!err) {
        quantiaHostEncontrado++;
        console.log(`${quantiaHostEncontrado} - host -> '${hostname}' | IP: ${address}`);
        console.log(`Palavras lidas: ${palavrasLidas}/${totalPalavras-palavrasLidas}`);
      }
      palavrasLidas++;
    });
  });
  console.log("Criado por Gabriel Logan")
});
